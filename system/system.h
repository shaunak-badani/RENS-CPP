#ifndef SYSTEM_H
#define SYSTEM_H

#include "units.h"
#include "utilities.h"
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

        void initializeVelocities();

        virtual float potentialEnergy() = 0;
        virtual float potentialEnergy(std::vector<std::vector<float>>& positions) = 0;

        virtual std::vector<std::vector<float>> force() = 0;
        virtual std::vector<std::vector<float>> force(std::vector<std::vector<float>>& positions) = 0;

        float kineticEnergy();

        virtual float kineticEnergy(std::vector<std::vector<float>> velocities);

        float totalEnergy();

        float instantaneousTemperature();

        float instantaneousTemperature(std::vector<std::vector<float>>& velocities);

        virtual void handleOutput(float timeStep, FileOperations* fileOpObject);

        // system based constraints

        virtual void systemConstraints();
};

#endif