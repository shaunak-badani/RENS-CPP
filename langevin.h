#ifndef LANGEVIN_H
#define LANGEVIN_H

#include "integrator.h"
#include "system.h"
#include "config.h"
#include "math.h"
#include "utilities.h"
#include "fileoperations.h"

class Langevin : public Integrator {
    public:
        float friction;
        float damping;
        float damping_2;
        bool enableOutput;

        Langevin() {
            this->dt = 0.001;
            this->friction = 0.05;
            this->damping = exp(-this->friction * this->dt);
            this->damping_2 = pow(this->damping, 2);
            this->enableOutput = true;
        }

        void step(System* sys, FileOperations* fileOpObject, int numSteps = 1) {

            float kT = temperature() * kB;
            float sigma;
            float randomNumberHolder;

            // Do units conversion in the case of LJ system
            if(!arbitrary)
                kT *= (kJ_mol_TO_J) / (AMU_TO_KG) * pow(M_S_TO_A_PS, 2);
            std::vector<std::vector<float>> force = sys->force();

            int N = sys->velocities.size();
            int d = sys->velocities[0].size();
            for(int n = 0 ; n < numSteps ; n++) {
                for(int i = 0 ; i < N ; i++) {
                    for(int j = 0 ; j < d ; j++) {
                        sys->velocities[i][j] += (this->dt / 2) * (force[i][j] / sys->masses[i]);
                        sys->positions[i][j] += (this->dt / 2) * sys->velocities[i][j];
                    }
                }

                for(int i = 0 ; i < N ; i++) {
                    for(int j = 0 ; j < d ; j++) {
                        sys->velocities[i][j] *= this->damping;
                        sigma = sqrt((kT / sys->masses[i]) * (1 - this->damping_2));
                        randomNumberHolder = generateNormalRandom(0, sigma);
                        // std::cout << (randomNumberHolder) << std::endl;
                        sys->velocities[i][j] += randomNumberHolder;

                        sys->positions[i][j] += (this->dt / 2) * sys->velocities[i][j];
                    }
                }


                force = sys->force();
                for(int i = 0 ; i < N ; i++) 
                    for(int j = 0 ; j < d ; j++)
                        sys->velocities[i][j] += (this->dt / 2) * (force[i][j] / sys->masses[i]);

                if(n % outputPeriod == 0 && enableOutput) {
                    sys->handleOutput((float)(n * this->dt), fileOpObject);
                }
            }

        }
};

#endif