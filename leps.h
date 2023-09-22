#ifndef LEPS_H
#define LEPS_H

#include "system.h"
#include "math.h"

class LEPS : public System {

    public:
        float alpha;
        float r0;
        float a;
        float b;
        float c;
        float dAB;
        float dBC;
        float dAC;

        LEPS();

        void setupSystem();

        void initializePositions();

        std::vector<std::vector<float>> force(std::vector<std::vector<float>>& pos);

        std::vector<std::vector<float>> force();

        float potentialEnergy(std::vector<std::vector<float>>& pos);

        float potentialEnergy();


    protected:
        float Q(float r, float d);

        float Q_der(float r, float d);

        float J(float r, float d);

        float J_der(float r, float d);

        float U(std::vector<float> point);

        std::vector<float> F(std::vector<float> point);
};

class LEPS2 : public LEPS {
    
    public:
        float kC;
        float rAC;

        LEPS2();

        std::vector<std::vector<float>> force(std::vector<std::vector<float>>& pos);

        std::vector<std::vector<float>> force();

        float potentialEnergy(std::vector<std::vector<float>>& pos);

        float potentialEnergy();

    protected:
        float U(std::vector<float> point);

        std::vector<float> F(std::vector<float> point);
};

#endif