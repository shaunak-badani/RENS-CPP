#include <iostream>
#include "smit.h"
#include "system.h"
#include "integrator.h"
#include "langevin.h"
#include "fileoperations.h"
#include "config.h"
#include "mpi.h"
#include "SystemFactory.h"
#include "PropagatorFactory.h"

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

    System* sys = SystemFactory::createSystem(systemName);

    Integrator* stepper = PropagatorFactory::instantiatePropagator(runType);
    
    FileOperations* fileOpObject = new FileOperations();

    stepper->step(sys, fileOpObject, numSteps);

    MPI_Finalize();
}