#include "remd.h"

#include "integrator.h"
#include "system.h"
#include "langevin.h"
#include "mpi.h"
#include "math.h"

REMDIntegrator::REMDIntegrator() {
    stepper = new Langevin();
    stepper->enableOutput = false;
    stepper->handleConstraints = false;
    this->dt = stepper->dt;
    this->exchangePeriod = 1000;
    MPI_Comm_rank( MPI_COMM_WORLD, &replicaNo );
    this->isMaster = (replicaNo == 0);
}

/* BEGIN MPI FUNCTIONS */

bool REMDIntegrator::attemptExchange(System* sys, int swapPartner) {
    float u_i = sys->potentialEnergy();
    float t_i = temperature();
    float t_j;
    float acceptance_probability = 1.0f;
    bool exchange = true;
    float scalingFactor = 1.0f;

    if(this->isMaster) {
        float u_j = 0;
        float delta;
        MPI_Recv(&u_j, 1, MPI_FLOAT, swapPartner, POTENTIAL_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&t_j, 1, MPI_FLOAT, swapPartner, TEMPERATURE_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE );

        delta = ( 1 / (kB * temperature()) - 1 / (kB * t_j)) * (u_j - u_i);
        if(delta > 0) {
            acceptance_probability = exp(-delta);
            if(generateUniformRandom() >= acceptance_probability)
                exchange = false;
        }

        MPI_Send( &exchange, 1, MPI_CXX_BOOL, swapPartner, EXCHANGE_TAG, MPI_COMM_WORLD );

        if(exchange) {
            MPI_Send( &t_i, 1, MPI_FLOAT, swapPartner, TEMPERATURE_TAG, MPI_COMM_WORLD);
            scalingFactor = sqrt(t_j / t_i);
        }

        // writeValuesToFile from master
    }

    if(!this->isMaster) {
        MPI_Send(&u_i, 1, MPI_FLOAT, swapPartner, POTENTIAL_TAG, MPI_COMM_WORLD );
        MPI_Send(&t_i, 1, MPI_FLOAT, swapPartner, TEMPERATURE_TAG, MPI_COMM_WORLD );

        MPI_Recv(&exchange, 1, MPI_CXX_BOOL, swapPartner, EXCHANGE_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if(exchange) {
            MPI_Recv(&t_j, 1, MPI_FLOAT, swapPartner, TEMPERATURE_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            scalingFactor = sqrt(t_j / temperature());
        }
    }
    
    int N = sys->velocities.size();
    int d = sys->velocities[0].size();

    for(int i = 0 ; i < N ; i++) 
        for(int j = 0 ; j < d ; j++)
            sys->velocities[i][j] *= scalingFactor;
    
    return exchange;
}

void REMDIntegrator::swapPhaseSpaceVectors(System* sys, bool swapPartner) {
    int N = sys->velocities.size();
    int d = sys->velocities[0].size();

    std::vector<std::vector<float>> temporaryStorageVector(
                                N, std::vector<float>(d, 0.0f));

    if(this->isMaster) {
        for(int i = 0 ; i < N ; i++) {
            MPI_Send( &(sys->positions[i][0]), d, MPI_FLOAT, swapPartner, POSITION_TAG_1, MPI_COMM_WORLD );
            MPI_Recv( &(temporaryStorageVector[i][0]), d, MPI_FLOAT, swapPartner, POSITION_TAG_2, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
        }
    }

    if(!this->isMaster) {
        for(int i = 0 ; i < N ; i++) {
            MPI_Recv( &(temporaryStorageVector[i][0]), d, MPI_FLOAT, swapPartner, POSITION_TAG_1, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
            MPI_Send( &(sys->positions[i][0]), d, MPI_FLOAT, swapPartner, POSITION_TAG_2, MPI_COMM_WORLD );
        }
    }

    sys->positions = temporaryStorageVector;

    if(this->isMaster) {
        for(int i = 0 ; i < N ; i++) {
            MPI_Send( &(sys->velocities[i][0]), d, MPI_FLOAT, swapPartner, POSITION_TAG_1, MPI_COMM_WORLD );
            MPI_Recv( &(temporaryStorageVector[i][0]), d, MPI_FLOAT, swapPartner, POSITION_TAG_2, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
        }
    }

    if(!this->isMaster) {
        for(int i = 0 ; i < N ; i++) {
            MPI_Recv( &(temporaryStorageVector[i][0]), d, MPI_FLOAT, swapPartner, POSITION_TAG_1, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
            MPI_Send( &(sys->velocities[i][0]), d, MPI_FLOAT, swapPartner, POSITION_TAG_2, MPI_COMM_WORLD );
        }
    }
    sys->velocities = temporaryStorageVector;
}

/* END MPI FUNCTIONS */


void REMDIntegrator::step(System* sys, FileOperations* fileOpObject, int numSteps) {


    for(int n = 0 ; n < numSteps ; n++) {
        stepper->step(sys, fileOpObject);

        // Attempt a replica exchange step
        if(n % this->exchangePeriod == 0) {
            int swapPartner = (1 - replicaNo);

            // calculate acceptance probability
            bool exchange = this->attemptExchange(sys, swapPartner);

            // Swap vectors if successful
            if(exchange) 
                swapPhaseSpaceVectors(sys, swapPartner);
            

            // Write exchanges to file
            if(this->isMaster) {
                fileOpObject->registerExchangesScalars("Src", this->replicaNo);
                fileOpObject->registerExchangesScalars("Dest", swapPartner);
                fileOpObject->registerExchangesScalars("Exchanged", exchange);
                fileOpObject->writeExchangesScalars((float)n * this->dt);
            }
        }

        sys->systemConstraints();

        if(n % outputPeriod == 0) {
            sys->handleOutput((float) n * this->dt, fileOpObject);
        }
    }
}