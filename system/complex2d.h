#ifndef COMPLEX_2D_H
#define COMPLEX_2D_H

#include "system.h"
#include "math.h"

class Complex2D : public System {

    public:

        Complex2D();

        void setupSystem();

        void initializePositions();

        float potentialEnergy(std::vector<std::vector<float>>& pos);

        float potentialEnergy();

        std::vector<std::vector<float>> force(std::vector<std::vector<float>>& pos);

        std::vector<std::vector<float>> force();
    
    protected:
        float U(std::vector<float>& pos);

        std::vector<float> F(std::vector<float>& pos);

};

#endif