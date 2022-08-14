#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iomanip>

class FileOperations {
    public:
        std::unordered_map<std::string, float> scalars;
        std::unordered_map<std::string, std::vector<std::vector<float>>> phaseVectors;
        std::ofstream positionFileStream;
        std::ofstream velocityFileStream;
        std::ofstream scalarFileStream;
        std::ofstream phaseVectorFileStream;

        bool headersWritten;
        FileOperations() {
            
            this->positionFileStream.open("./p.txt");
            this->velocityFileStream.open("./v.txt");
            this->scalarFileStream.open("./scalars.txt");
            this->phaseVectorFileStream.open("./end.rst");
            this->headersWritten = false;

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

        ~FileOperations() {
            this->positionFileStream.close();
            this->velocityFileStream.close();
            this->scalarFileStream.close();
            this->phaseVectorFileStream.close();
        }
};

#endif