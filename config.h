#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

float samplingTemperature;
std::vector<float> samplingTemperatures;
int numSteps;
int numParticles;
int outputPeriod;
std::string runType;
std::string systemName;
std::string rst;
bool isRstPresent;
float tauValue;

void setDefaultConfig() {
    samplingTemperature = 0.3;
    samplingTemperatures = {0.3f, 2.0f};
    numSteps = 1000;
    numParticles = 10;
    outputPeriod = 100;
    runType = "nve";
    systemName = "1D_Smit";
    tauValue = 0.01f;
    isRstPresent = false;
}

void getConfigFromFile(std::string fileName) {
            
    std::ifstream jsonFileObject(fileName);
    json data = json::parse(jsonFileObject);

    if(data.contains("temperature"))
        samplingTemperature = data["temperature"];
    
    if(data.contains("num_steps"))
        numSteps = data["num_steps"];

    if(data.contains("output_period"))
        outputPeriod = data["output_period"];

    if(data.contains("run_type"))
        runType = data["run_type"];

    if(data.contains("system"))
        systemName = data["system"];

    if(data.contains("tau"))
        tauValue = data["tau"];

    if(data.contains("rst")) {
        isRstPresent = true;
        rst = data["rst"];
    }

    if(data.contains("num_particles"))
        numParticles = data["num_particles"];

    // if(data.contains("temperatures"))
    //     samplingTemperatures = data["temperatures"];
}

float temperature() {
    int noOfReplicas;

    MPI_Comm_size( MPI_COMM_WORLD, &noOfReplicas );

    if(noOfReplicas > 1) {
        int rank;
        MPI_Comm_rank( MPI_COMM_WORLD, &rank );
        return samplingTemperatures[rank];
    }
    return samplingTemperature;
}
 

#endif