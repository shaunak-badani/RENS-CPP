#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "system.h"

class Integrator {
    public:
        float dt;
        
        virtual void step(System* sys, FileOperations* fileOpObject, int numSteps = 1) = 0;

};

#endif