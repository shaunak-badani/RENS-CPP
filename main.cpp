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
#include "rens_scaling.h"
#include "mpi.h"
#include "mullermod.h"
#include "leps.h"
#include "leonnardjones.h"
#include "complex2d.h"
#include "SystemFactory.h"

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

    Integrator* stepper;

    if(!runType.compare("nve"))
        stepper = new MicroCanonical();
    else if(!runType.compare("nvt")) 
        stepper = new Langevin();
    else if(!runType.compare("remd"))
        stepper = new REMDIntegrator();
    else if(!runType.compare("rens")) {
        if(useScalingProtocol)
            stepper = new RENSScalingIntegrator();
        else
            stepper = new RENSIntegrator();
    }
    else
        stepper = new MicroCanonical();

    FileOperations* fileOpObject = new FileOperations();

    stepper->step(sys, fileOpObject, numSteps);

    MPI_Finalize();
}