#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "system.h"

class Integrator {
    public:
        float dt;
        
        virtual void step(System* sys, int nSteps = 1) = 0;

};

#endif