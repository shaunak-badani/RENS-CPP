#ifndef MULLER_H
#define MULLER_H

#include "system.h"
#include "config.h"
#include "utilities.h"
#include "math.h"

class Muller : public System {
    public:
        std::vector<float> A;
        std::vector<float> xMean;
        std::vector<float> yMean;
        std::vector<float> a;
        std::vector<float> b;
        std::vector<float> c;

        Muller();

        void setupSystem();

        void initializePositions();

        std::vector<std::vector<float>> force(std::vector<std::vector<float>>& pos);

        std::vector<std::vector<float>> force();

        float potentialEnergy(std::vector<std::vector<float>>& pos);

        float potentialEnergy();

    protected:
        float U(std::vector<float> point);

        std::vector<float> F(std::vector<float> point);
};

class MullerMod : public Muller { 
    public:
        std::vector<float> center;
        float sigma, sigma_2;
        float height;

        MullerMod();

        /* Need the following functions, but are duplicate of the ones in Muller
        they don't call the protected function of MullerMod
        */

        std::vector<std::vector<float>> force(std::vector<std::vector<float>>& pos);

        std::vector<std::vector<float>> force();

        float potentialEnergy(std::vector<std::vector<float>>& pos);

        float potentialEnergy();

    protected:
        float U(std::vector<float> pos);

        std::vector<float> F(std::vector<float> pos);
};

#endif