#include <iostream>
#include "smit.h"
#include "system.h"
#include "integrator.h"
#include "microcanonical.h"
#include "langevin.h"
#include "fileoperations.h"

int main() {
    System* sys = new Smit();
    Integrator* stepper = new Langevin();
    FileOperations* fileOpObject = new FileOperations();

    // Config variables
    int numSteps = 1000;
    int outputPeriod = 100;

    for(int i = 0 ; i < numSteps ; i++) {
        stepper->step(sys);
        if(i % outputPeriod == 0) {
            sys->handleOutput((float)i * stepper->dt, fileOpObject);
        }
    }
}