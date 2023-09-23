#ifndef PROPAGATOR_FACTORY_H
#define PROPAGATOR_FACTORY_H
#include "integrator.h"
#include <string>

class PropagatorFactory {
    public:
        static Integrator* instantiatePropagator(std::string ensembleName);
};

#endif