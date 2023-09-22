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
        void setupSystem();

        void initializePositions();

        Smit();

        float potentialEnergy();
        float potentialEnergy(std::vector<std::vector<float>>& positions);

        std::vector<std::vector<float>> force();

        std::vector<std::vector<float>> force(std::vector<std::vector<float>>& positions);
    
    private:
        float U(float x);

        float F(float x);

};

#endif