#ifndef SYSTEM_H
#define SYSTEM_H

#include "units.h"
#include <vector>
#include <math.h>
#include "fileoperations.h"

class System {
    public:
        // Define max available number of particles as a constant
        // elsewhere, in some config file
        std::vector<std::vector<float>> positions;
        std::vector<std::vector<float>> velocities;
        std::vector<float> masses;

        int numberOfParticles;
        int systemDimensionality;

        virtual void setupSystem() = 0;
        virtual void initializePositions() = 0;
        virtual void initializeVelocities() = 0;

        virtual float potentialEnergy() = 0;
        virtual float potentialEnergy(std::vector<std::vector<float>>& positions) = 0;

        virtual std::vector<std::vector<float>> force() = 0;
        virtual std::vector<std::vector<float>> force(std::vector<std::vector<float>>& positions) = 0;

        float kineticEnergy() {
            return this->kineticEnergy(this->velocities);
        }

        float kineticEnergy(std::vector<std::vector<float>> velocities) {
            float mv2 = 0;
            for(int i = 0 ; i < this->numberOfParticles ; i++) {
                for(int j = 0 ; j < this->systemDimensionality ; j++) {
                    mv2 += this->masses[i] * pow(velocities[i][j], 2);
                }
            }
            return 0.5 * mv2;
        }

        float totalEnergy() {
            return this->potentialEnergy() + this->kineticEnergy();
        }

        float instantaneousTemperature() {
            return this->instantaneousTemperature(this->velocities);
        }

        float instantaneousTemperature(std::vector<std::vector<float>> velocities) {
            float KE = this->kineticEnergy(velocities);
            return 2 * KE / (this->numberOfParticles * this->systemDimensionality * kB);
        }

        void handleOutput(float timeStep, FileOperations* fileOpObject) {
            fileOpObject->registerScalarData("KE", this->kineticEnergy());
            fileOpObject->registerScalarData("PE", this->potentialEnergy());
            fileOpObject->registerScalarData("TE", this->totalEnergy());
            fileOpObject->registerScalarData("T", this->instantaneousTemperature());
            fileOpObject->writeScalarData(timeStep);

            fileOpObject->registerVectorData("p", this->positions);
            fileOpObject->registerVectorData("v", this->velocities);
            fileOpObject->writeVectorData(timeStep);
        }
};

#endif