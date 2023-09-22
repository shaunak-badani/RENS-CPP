#ifndef RENS_2_H
#define RENS_2_H

#include "rens.h"
#include "system.h"
#include "math.h"
#include "langevin.h"

class RENSScalingIntegrator : public RENSIntegrator {

    public:
        Langevin* rensLangevin;

        RENSScalingIntegrator();

        void setupRENS(System* sys);

        void performWorkSimulation(System* sys, FileOperations* fileOpObject, float timeStep);

};

#endif