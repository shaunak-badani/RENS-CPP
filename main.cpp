#include <iostream>
#include "smit.h"
#include "system.h"
#include "integrator.h"
#include "microcanonical.h"
#include "langevin.h"
#include "fileoperations.h"
#include "config.h"

int main() {

    setDefaultConfig();
    getConfigFromFile("langevin_2.json");


    System* sys = new Smit();
    Integrator* stepper = new Langevin();
    FileOperations* fileOpObject = new FileOperations();

    for(int i = 0 ; i < numSteps ; i++) {
        stepper->step(sys);
        if(i % outputPeriod == 0) {
            sys->handleOutput((float)i * stepper->dt, fileOpObject);
        }
    }
}