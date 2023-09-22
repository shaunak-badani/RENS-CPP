#ifndef LJ_H
#define LJ_H

#include "system.h"
#include "math.h"

class LeonnardJones : public System {
    public:
        float sigma;
        float L;
        float epsilon;
        float rC;

        LeonnardJones();

        void setupSystem();

        void initializePositions();

        void initializeVelocities();

        std::vector<std::vector<float>> force(std::vector<std::vector<float>>& pos);

        std::vector<std::vector<float>> force();

        float potentialEnergy(std::vector<std::vector<float>>& pos);

        float potentialEnergy();

        float kineticEnergy();

        float kineticEnergy(std::vector<std::vector<float>> velocities);

        void systemConstraints();

        void handleOutput(float timeStep, FileOperations* fileOpObject);
};

#endif