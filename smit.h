#ifndef SMIT_H
#define SMIT_H

#include <vector>

#include "system.h"
#include "units.h"
#include "utilities.h"
#include "math.h"
#include "config.h"

class Smit : public System {
    public:
        void setupSystem() {
            this->numberOfParticles = numParticles;
            this->systemDimensionality = 1;
            for(int i = 0 ; i < this->numberOfParticles ; i++)
                this->masses.push_back(1.0f);
            // read from rst file if required
        }

        void initializePositions() {
            this->positions = initializeRandomVector(this->numberOfParticles,
                                            this->systemDimensionality,
                                            -2.0f, 2.0f);
        }

        void initializeVelocities() {

            int N = 10;
            float sigma;

            for(int i = 0 ; i < this->numberOfParticles ; i++) {
                this->velocities.push_back(std::vector<float>(this->systemDimensionality));
                for(int j = 0 ; j < this->systemDimensionality ; j++) {
                    sigma = sqrt(kB * temperature() / this->masses[i]);
                    this->velocities[i][j] = generateNormalRandom(0.0f, sigma);
                }
            }

        }

        Smit() {
            this->setupSystem();
            this->initializePositions();
            this->initializeVelocities();
        }

        float potentialEnergy() {
            return this->potentialEnergy(this->positions);
        }

        float potentialEnergy(std::vector<std::vector<float>>& positions) {
            float totalPE = 0;
            for(int i = 0 ; i < this->numberOfParticles ; i++) {
                totalPE += this->U(positions[i][0]);
            }
            return totalPE;
        }

        std::vector<std::vector<float>> force() {
            std::vector<std::vector<float>> force_vector(
                this->numberOfParticles,
                std::vector<float>(this->systemDimensionality, 0)
            );

            for(int i = 0 ; i < this->numberOfParticles ; i++) {
                force_vector[i][0] = this->F(this->positions[i][0]);
            }
            return force_vector;
        }

        std::vector<std::vector<float>> force(std::vector<std::vector<float>>& positions) {
            std::vector<std::vector<float>> force_vector(
                this->numberOfParticles,
                std::vector<float>(this->systemDimensionality, 0)
            );

            for(int i = 0 ; i < this->numberOfParticles ; i++) {
                force_vector[i][0] = this->F(positions[i][0]);
            }
            return force_vector;
        }
    
    private:
        float U(float x) {
            if (x < -1.25)
                return 4 * pow(M_PI, 2) * pow((x + 1.25), 2);

            if (x < -0.25)
                return 2 * (1 + sin(2 * M_PI * x));

            if (x < 0.75)
                return 3 * (1 + sin(2 * M_PI * x));
            
            if (x < 1.75)
                return 4 * (1 + sin(2 * M_PI * x));
            
            return 8 * pow(M_PI, 2) * pow((x - 1.75), 2);
        }

        float F(float x) {
            if (x < -1.25)
                return -8 * pow(M_PI, 2) * (x + 1.25);

            if (x < -0.25)
                return -4 * M_PI * cos(2 * M_PI * x);

            if (x < 0.75)
                return -6 * M_PI * cos(2 * M_PI * x);
            
            if (x < 1.75)
                return -8 * M_PI * cos(2 * M_PI * x);
            
            return -16 * pow(M_PI, 2) * (x - 1.75);
        }

};

#endif