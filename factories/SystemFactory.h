#ifndef SYSTEM_FACTORY_H
#define SYSTEM_FACTORY_H
#include "system.h"

class SystemFactory {
    public:
        static System* createSystem(std::string name);
};

#endif