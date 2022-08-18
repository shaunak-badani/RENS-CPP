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
#include "mullermod.h"
#include "leps.h"
#include "leonnardjones.h"

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

    System* sys;
    if(!systemName.compare("1D_Smit"))
        sys = new Smit();
    else if(!systemName.compare("Muller"))
        sys = new Muller();
    else if(!systemName.compare("MullerMod"))
        sys = new MullerMod();
    else if(!systemName.compare("LEPS_I"))
        sys = new LEPS();
    else if(!systemName.compare("LEPS_II"))
        sys = new LEPS2();
    else if(!systemName.compare("LJ"))
        sys = new LeonnardJones();
    else
        sys = new Smit();

    Integrator* stepper;

    if(!runType.compare("nve"))
        stepper = new MicroCanonical();
    else if(!runType.compare("nvt")) 
        stepper = new Langevin();
    else if(!runType.compare("remd"))
        stepper = new REMDIntegrator();
    else if(!runType.compare("rens")) 
        stepper = new RENSIntegrator();
    else
        stepper = new MicroCanonical();

    FileOperations* fileOpObject = new FileOperations();

    stepper->step(sys, fileOpObject, numSteps);

    MPI_Finalize();
}