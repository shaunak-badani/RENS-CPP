#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>
#include "json.hpp"
#include <mpi.h>

using json = nlohmann::json;

extern float samplingTemperature;
extern std::vector<float> samplingTemperatures;
extern int numSteps;
extern int numParticles;
extern int outputPeriod;
extern std::string runType;
extern std::string runName;
extern std::string systemName;
extern std::string rst;
extern bool isRstPresent;
extern bool ada;
extern float tauValue;
extern bool useScalingProtocol;

float temperature();

void setDefaultConfig();

void printConfig(int rank);

void getConfigFromFile(std::string fileName);

#endif