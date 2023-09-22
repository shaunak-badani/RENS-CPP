#ifndef RENS_INTEGRATOR_H
#define RENS_INTEGRATOR_H

#include "remd.h"
#include "config.h"
#include "mpi.h"
#include "math.h"
#include "utilities.h"


#define TEMP_TAG 64
#define WORK_TAG 65
#define DATA_VALUES_TAG 66
#define ATTEMPT_TAG 67


class RENSIntegrator : public REMDIntegrator {

    public:
        float tau;
        float attemptRate;
        int andersenUpdateFrequency;
        float numWorkSimulationSteps;
        bool mode;
        float T_A;
        float T_B;
        float w;
        float heat;
        int currentStep;
        float peerReplicaNo;
        int noOfReplicas;
        std::vector<std::vector<float>> initialPositions;
        std::vector<std::vector<float>> initialVelocities;

        RENSIntegrator();

        /* RENS FUNCTIONS */

        virtual void setupRENS(System* sys);
        
        virtual void performWorkSimulation(System* sys, FileOperations* fileOpObject, float timeStep);

        bool determineExchange(System* sys, FileOperations* fileOpObject, float timeStep);

        void writeExchanges( std::vector<float> dataValues, FileOperations* fileOpObject, float timeStep );

        void andersenUpdate(System* sys, float T_lambda);

        void attemptSwitching(System* sys);

        /* END RENS FUNCTIONS */

        void step(System* sys, FileOperations* fileOpObject, int numSteps = 1);
};

#endif