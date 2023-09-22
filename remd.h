#ifndef REMD_INTEGRATOR_H
#define REMD_INTEGRATOR_H

#define POTENTIAL_TAG 60
#define TEMPERATURE_TAG 61
#define EXCHANGE_TAG 62
#define POSITION_TAG_1 63
#define POSITION_TAG_2 64

#include "integrator.h"
#include "system.h"
#include "langevin.h"
#include "mpi.h"
#include "math.h"

class REMDIntegrator : public Integrator {
    public:
        Langevin* stepper;
        int exchangePeriod;
        int replicaNo;
        bool isMaster;

        REMDIntegrator();

        /* BEGIN MPI FUNCTIONS */

        bool attemptExchange(System* sys, int swapPartner);
        void swapPhaseSpaceVectors(System* sys, bool swapPartner);
        
        /* END MPI FUNCTIONS */


        void step(System* sys, FileOperations* fileOpObject, int numSteps = 1);
};

#endif