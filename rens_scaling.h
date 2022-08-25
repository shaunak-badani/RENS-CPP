#ifndef RENS_2_H
#define RENS_2_H

#include "rens.h"
#include "system.h"
#include "math.h"
#include "langevin.h"

class RENSScalingIntegrator : public RENSIntegrator {

    public:
        Langevin* rensLangevin;

        RENSScalingIntegrator() {
            rensLangevin = new Langevin();
            rensLangevin->enableOutput = false;
            rensLangevin->handleConstraints = false;
            rensLangevin->useConfigTemperature = false;
            rensLangevin->temp = temperature();
        }

        void setupRENS(System* sys) {
            this->T_A = temperature();

            this->currentStep = 0;
            this->initialPositions = sys->positions;
            this->initialVelocities = sys->velocities;

            this->w = 0.0f;
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

        void performWorkSimulation(System* sys, FileOperations* fileOpObject, float timeStep) {
            
            std::vector<std::vector<float>> v = sys->velocities;
            int N = v.size();
            int d = v[0].size();

            if(this->numWorkSimulationSteps == 0) {
                this->heat += sys->totalEnergy() / (kB * this->T_A);
                for(int i = 0 ; i < N ; i++) 
                    for(int j = 0 ; j < d ; j++)
                        v[i][j] *= sqrt(this->T_B / this->T_A);
                this->heat -= (sys->kineticEnergy(v) + sys->potentialEnergy()) / (kB * this->T_B);
            }

            if(this->currentStep >= this->numWorkSimulationSteps) {
                this->mode = 0;
                float N_f = sys->numberOfParticles * sys->systemDimensionality;
                this->heat += (N_f / 2) * log(this->T_B / this->T_A);
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
            float lambda, T_prev, T_next, T_lambda;
            if(this->currentStep % this->andersenUpdateFrequency == 0) {
                lambda = ((float) this->currentStep * this->dt) / this->tau;
                T_prev = this->T_A + lambda * (this->T_B - this->T_A);
                this->heat += (sys->totalEnergy()) / (kB * T_prev);

                lambda = ((float) (this->currentStep + this->andersenUpdateFrequency) * this->dt) / this->tau;
                T_next = this->T_A + lambda * (this->T_B - this->T_A);                    

                for(int i = 0 ; i < sys->numberOfParticles ; i++) {
                    for(int j = 0 ; j < sys->systemDimensionality ; j++) {
                        sys->velocities[i][j] *= sqrt(T_next / T_prev);
                    }
                }
                this->heat -= sys->totalEnergy() / (kB * T_next);
                this->rensLangevin->temp = T_next;
            }
            else {
                this->rensLangevin->step(sys, fileOpObject);
            }
            this->currentStep++;
        }

};

#endif