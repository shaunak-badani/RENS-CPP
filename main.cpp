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
#include "mpi.h"

int main(int argc, char **argv) {

    MPI_Init(&argc, &argv);
    setDefaultConfig();
    int rank;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );

    if(argc > 1) {
        getConfigFromFile(argv[1]);
    }
    else {
        if(rank == 0)
            std::cout << "No config file passed. Defaulting to standard configs..." << std::endl;
    }
    printConfig(rank);

    System* sys = new Smit();
    Integrator* stepper;

    if(!runType.compare("nve"))
        stepper = new MicroCanonical();
    if(!runType.compare("nvt")) 
        stepper = new Langevin();
    if(!runType.compare("remd"))
        stepper = new REMDIntegrator();
    if(!runType.compare("rens")) 
        stepper = new RENSIntegrator();
    
    FileOperations* fileOpObject = new FileOperations();

    stepper->step(sys, fileOpObject, numSteps);
    MPI_Finalize();
}