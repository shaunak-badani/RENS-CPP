#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iomanip>
#include "mpi.h"
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
        FileOperations() {
            this->folderName = runName;
            int nReplicas;
            MPI_Comm_size( MPI_COMM_WORLD, &nReplicas );
            this->intermediateFolder = ".";

            if(nReplicas > 1) {
                int rank;
                MPI_Comm_rank( MPI_COMM_WORLD, &rank ); 
                this->intermediateFolder = std::to_string(rank);
            }

            if(ada) {
                this->folderName = "/scratch/shaunak/CPPRun/" + this->folderName;
            }

            // int statusMessage = mkdir(this->folderName.c_str(), 0777);
            // if(statusMessage == -1) {
            //     std::cerr << "Error while creating directory" << std::endl;
            // }
            std::string cmd = "mkdir -p " + this->folderName + "/" + this->intermediateFolder;
            system(cmd.c_str());

            std::string genericFolderPath = this->folderName + "/" + this->intermediateFolder;

            
            this->positionFileStream.open(genericFolderPath + "/p.txt");
            this->velocityFileStream.open(genericFolderPath + "/v.txt");
            this->scalarFileStream.open(genericFolderPath + "/scalars.txt");
            this->phaseVectorFileStream.open(genericFolderPath + "/end.rst");
            this->headersWritten = false;

            this->exchangesFileStream.open(this->folderName + "/exchanges.txt");
            this->exchangesHeaders = false;
        }

        void registerScalarData(std::string key, float value) {
            // if(scalars.contains(key))
            //     std::cout << "The hashmap contains the key. Overriding..." << std::endl;
            scalars[key] = value;
        }

        void registerVectorData(std::string key, std::vector<std::vector<float>> value) {
            if(!(key.compare("p")) && !(key.compare("v"))) {
                std::cout << "Only p and v keys allowed! Aborting registration of vector..." << std::endl;
                return;
            }
            // if(phaseVectors.contains(key))
            //     std::cout << "The hashmap contains the key. Overriding..." << std::endl;
            phaseVectors[key] = value;
        } 

        void writeScalarData(float timeStep) {
            if(!(this->headersWritten)) {
                (this->scalarFileStream) << "Time" << " ";
                for(auto scalarPair:this->scalars) 
                    (this->scalarFileStream) << scalarPair.first << " ";
                (this->scalarFileStream) << std::endl;
                this->headersWritten = true;
            }
            (this->scalarFileStream) << std::fixed << std::setprecision(3);
            (this->scalarFileStream) << timeStep << " ";

            (this->scalarFileStream) << std::setprecision(5);
            for(auto scalarPair:this->scalars) {
                (this->scalarFileStream) << scalarPair.second << " ";
            }
            (this->scalarFileStream) << std::endl;
        }

        void writeVectorData(float timeStep) {
            (this->positionFileStream) << std::fixed << std::setprecision(3);
            (this->velocityFileStream) << std::fixed << std::setprecision(3);

            (this->positionFileStream) << timeStep << " ";
            (this->velocityFileStream) << timeStep << " ";
            
            int N = this->phaseVectors["p"].size();
            int d = this->phaseVectors["p"][0].size();

            (this->positionFileStream) << std::setprecision(5);
            (this->velocityFileStream) << std::setprecision(5);

            for(int i = 0 ; i < N ; i++)
                for(int j = 0 ; j < d ; j++)    
                    (this->positionFileStream) << this->phaseVectors["p"][i][j] << " ";
            (this->positionFileStream) << std::endl;

            for(int i = 0 ; i < N ; i++)
                for(int j = 0 ; j < d ; j++)    
                    (this->velocityFileStream) << this->phaseVectors["v"][i][j] << " ";
            (this->velocityFileStream) << std::endl;
        }

        /* REMD + RENS SCALARS */

        void registerExchangesScalars(std::string key, float value) {
            exchangesScalars[key] = value;
        }

        void writeExchangesScalars(float timeStep) {
            if(!(this->exchangesHeaders)) {
                (this->exchangesFileStream) << "Time" << " ";
                for(auto scalarPair:this->exchangesScalars) 
                    (this->exchangesFileStream) << scalarPair.first << " ";
                (this->exchangesFileStream) << std::endl;
                this->exchangesHeaders = true;
            }
            (this->exchangesFileStream) << std::fixed << std::setprecision(3);
            (this->exchangesFileStream) << timeStep << " ";

            (this->exchangesFileStream) << std::setprecision(5);
            for(auto scalarPair:this->exchangesScalars) {
                (this->exchangesFileStream) << scalarPair.second << " ";
            }
            (this->exchangesFileStream) << std::endl;
        }

        /* END REMD + RENS SCALARS */

        ~FileOperations() {
            this->positionFileStream.close();
            this->velocityFileStream.close();
            this->scalarFileStream.close();
            this->phaseVectorFileStream.close();

            if(this->exchangesHeaders)
                this->exchangesFileStream.close();
        }
};

#endif