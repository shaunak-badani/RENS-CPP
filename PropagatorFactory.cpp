#include "PropagatorFactory.h"
#include "integrator.h"
#include "remd.h"
#include "rens.h"
#include "rens_scaling.h"
#include "microcanonical.h"

Integrator* getRensPropagator(bool useScalingProtocol)
{
    if(useScalingProtocol)
        return new RENSScalingIntegrator();
    return new RENSIntegrator();
}

Integrator* PropagatorFactory::instantiatePropagator(std::string ensembleName)
{
    if(!ensembleName.compare("nve"))
        return new MicroCanonical();

    if(!runType.compare("nvt")) 
        return new Langevin();

    if(!runType.compare("remd"))
        return new REMDIntegrator();

    if(!runType.compare("rens"))
        return new RENSIntegrator();
    
    if(!runType.compare("rens_scaling"))
        return new RENSScalingIntegrator();

    return new MicroCanonical();
}