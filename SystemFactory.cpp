#include "SystemFactory.h"
#include "system.h"
#include "smit.h"
#include "mullermod.h"
#include "leps.h"
#include "leonnardjones.h"
#include "complex2d.h"


System* SystemFactory::createSystem(std::string name)
{
    if(!name.compare("1D_Smit"))
        return new Smit();

    if(!name.compare("Muller"))
        return new Muller();

    if(!name.compare("MullerMod"))
        return new MullerMod();

    if(!name.compare("LEPS_I"))
        return new LEPS();

    if(!name.compare("LEPS_II"))
        return new LEPS2();

    if(!name.compare("LJ"))
        return new LeonnardJones();

    if(!systemName.compare("Complex_2D"))
        return new Complex2D();
    
    std::cout << "No system matched the names supported. Defaulting to Smit" << std::endl;
    return new Smit();
}