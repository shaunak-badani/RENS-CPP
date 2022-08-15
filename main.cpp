#include <iostream>
#include "smit.h"
#include "system.h"
#include "integrator.h"
#include "microcanonical.h"
#include "langevin.h"
#include "fileoperations.h"
#include "config.h"
#include "remd.h"
#include "rens.h"

int main(int argc, char **argv) {

    MPI_Init(&argc, &argv);
    setDefaultConfig();
    getConfigFromFile("langevin_2.json");

    System* sys = new Smit();
    Integrator* stepper = new RENSIntegrator();
    FileOperations* fileOpObject = new FileOperations();

    stepper->step(sys, fileOpObject, numSteps);
    MPI_Finalize();
}