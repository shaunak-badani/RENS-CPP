#ifndef MICROCANONICAL_H
#define MICROCANONICAL_H

#include "integrator.h"

class MicroCanonical : public Integrator {
    public:
        MicroCanonical();
        void step(System* sys, FileOperations* fileOpObject, int numSteps = 1);
};

#endif