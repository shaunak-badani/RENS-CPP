#include <iostream>
#include "smit.h"
#include "system.h"
#include "integrator.h"
#include "microcanonical.h"

int main() {
    System* sys = new Smit();
    Integrator* stepper = new MicroCanonical();

    // Config variables
    int numSteps = 1000;
    int outputPeriod = 100;

    for(int i = 0 ; i < numSteps ; i++) {
        stepper->step(sys);
        if(i % outputPeriod == 0) {
            std::cout << sys->totalEnergy() << std::endl;
        }
    }
}