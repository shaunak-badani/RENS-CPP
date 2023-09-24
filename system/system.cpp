#include "system.h"

#include "units.h"
#include "utilities.h"
#include <vector>
#include <math.h>
#include "fileoperations.h"



void System::initializeVelocities() {
    float sigma;
    for(int i = 0 ; i < this->numberOfParticles ; i++) {
        this->velocities.push_back(std::vector<float>(this->systemDimensionality, 0.0f));
        for(int j = 0 ; j < this->systemDimensionality ; j++) {
            sigma = sqrt(kB * temperature() / this->masses[i]);
            this->velocities[i][j] = generateNormalRandom(0.0f, sigma);
        }
    }
}

float System::kineticEnergy() {
    return this->kineticEnergy(this->velocities);
}

float System::kineticEnergy(std::vector<std::vector<float>> velocities) {
    float mv2 = 0;
    for(int i = 0 ; i < this->numberOfParticles ; i++) {
        for(int j = 0 ; j < this->systemDimensionality ; j++) {
            mv2 += this->masses[i] * pow(velocities[i][j], 2);
        }
    }
    return 0.5 * mv2;
}

float System::totalEnergy() {
    return this->potentialEnergy() + this->kineticEnergy();
}

float System::instantaneousTemperature() {
    return this->instantaneousTemperature(this->velocities);
}

float System::instantaneousTemperature(std::vector<std::vector<float>>& velocities) {
    float KE = this->kineticEnergy(velocities);
    return 2 * KE / (this->numberOfParticles * this->systemDimensionality * kB);
}

void System::handleOutput(float timeStep, FileOperations* fileOpObject) {
    fileOpObject->registerScalarData("KE", this->kineticEnergy());
    fileOpObject->registerScalarData("PE", this->potentialEnergy());
    fileOpObject->registerScalarData("TE", this->totalEnergy());
    fileOpObject->registerScalarData("T", this->instantaneousTemperature());
    fileOpObject->writeScalarData(timeStep);

    fileOpObject->registerVectorData("p", this->positions);
    fileOpObject->registerVectorData("v", this->velocities);
    fileOpObject->writeVectorData(timeStep);
}

void System::systemConstraints() {

}