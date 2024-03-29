#include "rens.h"
#include "config.h"
#include "mpi.h"
#include "math.h"
#include "utilities.h"


RENSIntegrator::RENSIntegrator() {
    this->tau = tauValue;
    this->attemptRate = 0.166;
    this->mode = 0;
    this->numWorkSimulationSteps = round(this->tau / this->dt);
    this->andersenUpdateFrequency = 500;

    MPI_Comm_size( MPI_COMM_WORLD, &(this->noOfReplicas));
}

/* RENS FUNCTIONS */

void RENSIntegrator::setupRENS(System* sys) {
    this->T_A = temperature();
    this->w = -sys->totalEnergy() / (kB * this->T_A);

    this->currentStep = 0;
    this->initialPositions = sys->positions;
    this->initialVelocities = sys->velocities;

    
    this->heat = 0.0f;
    
    // Need to exchange temperatures between peer and self
    if(peerReplicaNo >= 0 && peerReplicaNo < this->noOfReplicas) {
        if(this->replicaNo > peerReplicaNo) {
            MPI_Send( &(this->T_A), 1, MPI_FLOAT, peerReplicaNo, TEMP_TAG, MPI_COMM_WORLD );
            MPI_Recv( &(this->T_B), 1, MPI_FLOAT, peerReplicaNo, TEMP_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
        }
        else {
            MPI_Recv( &(this->T_B), 1, MPI_FLOAT, peerReplicaNo, TEMP_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
            MPI_Send( &(this->T_A), 1, MPI_FLOAT, peerReplicaNo, TEMP_TAG, MPI_COMM_WORLD );
        }
    }
}

void RENSIntegrator::performWorkSimulation(System* sys, FileOperations* fileOpObject, float timeStep) {
    std::vector<std::vector<float>> v = sys->velocities;
    int N = v.size();
    int d = v[0].size();

    if(this->numWorkSimulationSteps == 0) {
        for(int i = 0 ; i < N ; i++) 
            for(int j = 0 ; j < d ; j++)
                v[i][j] *= sqrt(this->T_B / this->T_A);
    }

    // Terminate the work simulation
    // And gather non-equilibrium information
    if( this->currentStep >= this->numWorkSimulationSteps ) {
        this->mode = 0;
        float N_f = N * d;
        this->heat += (N_f / 2) * log(this->T_B / this->T_A);

        this->w += (sys->kineticEnergy(v) + sys->potentialEnergy()) / (kB * T_B);
        this->w -= this->heat;


        bool exchange = this->determineExchange(sys, fileOpObject, timeStep);

        if(!exchange) {
            sys->positions = this->initialPositions;
            sys->velocities = this->initialVelocities;
        }
        else {
            sys->velocities = v;
            this->swapPhaseSpaceVectors(sys, peerReplicaNo);
        }

        return;
    }

    // Perform the work simulation if currentStep < totalWorkSimulationSteps
    float t = (this->currentStep * this->dt);
    float lambda = (t / this->tau);
    float T_lambda = this->T_A + lambda * (this->T_B - this->T_A);

    if(this->currentStep % this->andersenUpdateFrequency == 0) {
        float oldReducedHamiltonian = sys->totalEnergy() / (kB * T_lambda);
        this->andersenUpdate(sys, T_lambda);
        float newReducedHamiltonian = sys->totalEnergy() / (kB * T_lambda);
        this->heat += (newReducedHamiltonian - oldReducedHamiltonian);
    }
    else {
        float z = (this->T_B - this->T_A) * (1 / this->tau) / (2 * T_lambda);
        float z_exp = exp(z * this->dt / 2);

        std::vector<std::vector<float>> force = sys->force();
        for(int i = 0 ; i < N ; i++) {
            for(int j = 0 ; j < d ; j++) {
                sys->velocities[i][j] = sys->velocities[i][j] * z_exp
                    + force[i][j] * (z_exp - 1) / (z * sys->masses[i]);
                sys->positions[i][j] += sys->velocities[i][j] * this->dt;
            }
        }

        force = sys->force();
        for(int i = 0 ; i < N ; i++) {
            for(int j = 0 ; j < d ; j++) {
                sys->velocities[i][j] = sys->velocities[i][j] * z_exp
                    + force[i][j] * (z_exp - 1) / (z * sys->masses[i]);
            }
        }
    }
    this->currentStep++;
}

bool RENSIntegrator::determineExchange(System* sys, FileOperations* fileOpObject, float timeStep) {

    float wA = this->w;
    float wB = 0;
    bool exchange = false;
    float acceptanceProbability = 1.0f;
    std::vector<float> dataValues(8, 0.0f);

    // arr = []
    if(this->peerReplicaNo >= 0 && this->peerReplicaNo < this->noOfReplicas) {

        if(this->replicaNo < this->peerReplicaNo) {
            MPI_Recv( &(wB), 1, MPI_FLOAT, this->peerReplicaNo, WORK_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE );

            float w = wA + wB;
            
            if(w < 0) {
                exchange = true;
            }
            else {
                acceptanceProbability = exp(-w);
                float rand = generateUniformRandom();

                if(rand <= acceptanceProbability)
                    exchange = true;
            }

            dataValues = {timeStep, (float)this->replicaNo, 
                        (float)this->peerReplicaNo, (float)exchange, acceptanceProbability,
                        w, wA, wB};
            
            MPI_Send( &(dataValues[0]), (int)dataValues.size(), MPI_FLOAT, this->peerReplicaNo,
                            DATA_VALUES_TAG, MPI_COMM_WORLD );

            // send arr to peerRank
        }
        else {
            MPI_Send( &wA, 1, MPI_FLOAT, this->peerReplicaNo, WORK_TAG, MPI_COMM_WORLD );
            MPI_Recv( &(dataValues[0]), 8, MPI_FLOAT, this->peerReplicaNo,
                        DATA_VALUES_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
            exchange = dataValues[3];
        }
    }

    if(this->replicaNo % 2 != 0 && dataValues.size() > 0) {
        MPI_Send( &(dataValues[0]), (int)dataValues.size(), MPI_FLOAT, 0,
                            DATA_VALUES_TAG, MPI_COMM_WORLD );  
    }

    if(this->replicaNo == 0) {
        for(int i = 1 ; i < this->noOfReplicas ; i += 2) {
            MPI_Recv( &(dataValues[0]), 8, MPI_FLOAT, i,
                        DATA_VALUES_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
            this->writeExchanges(dataValues, fileOpObject, timeStep );
        } 
    }
    return exchange;
}

void RENSIntegrator::writeExchanges( std::vector<float> dataValues, FileOperations* fileOpObject, float timeStep ) {
    fileOpObject->registerExchangesScalars("Time", dataValues[0]);
    fileOpObject->registerExchangesScalars("Src", dataValues[1]);
    fileOpObject->registerExchangesScalars("Dest", dataValues[2]);
    fileOpObject->registerExchangesScalars("Exchanged", dataValues[3]);
    fileOpObject->registerExchangesScalars("Prob", dataValues[4]);
    fileOpObject->registerExchangesScalars("w", dataValues[5]);
    fileOpObject->registerExchangesScalars("w_a", dataValues[6]);
    fileOpObject->registerExchangesScalars("w_b", dataValues[7]);
    fileOpObject->writeExchangesScalars(timeStep);
}

void RENSIntegrator::andersenUpdate(System* sys, float T_lambda) {
    int N = sys->velocities.size();
    int d = sys->velocities[0].size();

    int indexForVelocityAssignment = generateUniformRandomInt(0, N);
    float beta = 1 / (kB * T_lambda);

    float sigma = 1 / sqrt(sys->masses[indexForVelocityAssignment] * beta);

    // change sigma for non arbitrary units
    if(!arbitrary)
        sigma *= sqrt(kJ_mol_TO_J / AMU_TO_KG) * M_S_TO_A_PS;

    for(int j = 0 ; j < d ; j++)
        sys->velocities[indexForVelocityAssignment][j] =
            generateNormalRandom(0, sigma);
}

void RENSIntegrator::attemptSwitching(System* sys) {
    // MPI_Barrier( MPI_COMM_WORLD );
    if(this->replicaNo % 2 == 0)
        this->peerReplicaNo = this->replicaNo + 1;
    else
        this->peerReplicaNo = this->replicaNo - 1;

    bool startWorkSimulation = false;
    if(this->peerReplicaNo >= 0 && this->peerReplicaNo < this->noOfReplicas) {
        if(this->replicaNo > this->peerReplicaNo) {
            float currentRandom = generateUniformRandom();
            if(currentRandom <= this->attemptRate * this->dt)
                startWorkSimulation = true;
            
            MPI_Send( &startWorkSimulation, 1, MPI_CXX_BOOL, this->peerReplicaNo,
                                ATTEMPT_TAG, MPI_COMM_WORLD );
        }
        else {
            MPI_Recv( &startWorkSimulation, 1, MPI_CXX_BOOL, this->peerReplicaNo,
                            ATTEMPT_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
        }
    }
    if(startWorkSimulation) {
        this->mode = 1;
        this->setupRENS(sys);
    }
}

/* END RENS FUNCTIONS */

void RENSIntegrator::step(System* sys, FileOperations* fileOpObject, int numSteps) {
    for(int i = 0 ; i < numSteps ; i++) {
        if(!this->mode) {
            stepper->step(sys, fileOpObject);
            this->attemptSwitching(sys);
            
            if(i % outputPeriod == 0)
                sys->handleOutput((float) i * this->dt, fileOpObject);
        }
        else {
            this->performWorkSimulation(sys, fileOpObject, (float) i * this->dt);
        }
        sys->systemConstraints();
    }
}