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
std::string runName;
std::string systemName;
std::string rst;
bool isRstPresent;
bool ada;
float tauValue;
bool useScalingProtocol;

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

void setDefaultConfig() {
    samplingTemperature = 0.3;
    samplingTemperatures = {0.3f, 2.0f};
    numSteps = 1000;
    numParticles = 1;
    outputPeriod = 100;
    runType = "nve";
    runName = "currentRun";
    systemName = "1D_Smit";
    tauValue = 0.024f;
    isRstPresent = false;
    ada = false;
    useScalingProtocol = false;
}

void printConfig(int rank) {
    std::cout << "Temperature : " << temperature() << std::endl;

    if(rank == 0) {
        std::cout << "Number of steps : " << numSteps << std::endl;
        std::cout << "Number of particles : " << numParticles << std::endl;
        std::cout << "Output Period : " << outputPeriod << std::endl;
        std::cout << "Run Type : " << runType << std::endl;
        std::cout << "System : " << systemName << std::endl;
        std::cout << "Tau Value : " << tauValue << std::endl;
        std::cout << "Use Scaling Protocol : " << useScalingProtocol << std::endl;
    }
   
}

void getConfigFromFile(std::string fileName) {
            
    std::ifstream jsonFileObject(fileName);
    json data = json::parse(jsonFileObject);

    if(data.contains("temperature"))
        samplingTemperature = data["temperature"];

    if(data.contains("temperatures")) {
        for(int i = 0 ; i < data["temperatures"].size() ; i++)
        samplingTemperatures[i] = data["temperatures"][i];
    }
    
    if(data.contains("num_steps"))
        numSteps = data["num_steps"];

    if(data.contains("output_period"))
        outputPeriod = data["output_period"];

    if(data.contains("run_type"))
        runType = data["run_type"];
    
    if(data.contains("run_name"))
        runName = data["run_name"];

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
    
    if(data.contains("ada"))
        ada = data["ada"];

    if(data.contains("use_scaling_protocol"))
        useScalingProtocol = data["use_scaling_protocol"];
    
    

    // if(data.contains("temperatures"))
    //     samplingTemperatures = data["temperatures"];
}


 

#endif