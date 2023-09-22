#ifndef LANGEVIN_H
#define LANGEVIN_H

#include "integrator.h"
#include "system.h"
#include "config.h"
#include "math.h"
#include "utilities.h"
#include "fileoperations.h"

class Langevin : public Integrator {
    public:
        float friction;
        float damping;
        float damping_2;
        bool enableOutput;
        bool handleConstraints;
        float temp;
        bool useConfigTemperature;

        Langevin();

        void step(System* sys, FileOperations* fileOpObject, int numSteps = 1);
};

#endif