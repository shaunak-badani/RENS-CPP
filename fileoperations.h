#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sys/stat.h>
#include <sys/types.h>
#include "config.h"


class FileOperations {
    public:
        std::unordered_map<std::string, float> scalars;
        std::unordered_map<std::string, std::vector<std::vector<float>>> phaseVectors;
        std::ofstream positionFileStream;
        std::ofstream velocityFileStream;
        std::ofstream scalarFileStream;
        std::ofstream phaseVectorFileStream;
        std::string folderName;
        std::string intermediateFolder;

        std::ofstream exchangesFileStream;
        std::unordered_map<std::string, float> exchangesScalars;
        bool exchangesHeaders;

        bool headersWritten;
        FileOperations();

        void registerScalarData(std::string key, float value);
        void registerVectorData(std::string key, std::vector<std::vector<float>> value);

        void writeScalarData(float timeStep);

        void writeVectorData(float timeStep);

        void writeXYZfiles(float timestep);

        /* REMD + RENS SCALARS */

        void registerExchangesScalars(std::string key, float value);

        void writeExchangesScalars(float timeStep);

        /* END REMD + RENS SCALARS */

        ~FileOperations();
};

#endif