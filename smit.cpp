#include "smit.h"

#include <vector>

#include "system.h"
#include "units.h"
#include "utilities.h"
#include "math.h"
#include "config.h"

void Smit::setupSystem() {
    this->numberOfParticles = numParticles;
    this->systemDimensionality = 1;
    for(int i = 0 ; i < this->numberOfParticles ; i++)
        this->masses.push_back(1.0f);
    // read from rst file if required
}

void Smit::initializePositions() {
    this->positions = initializeRandomVector(this->numberOfParticles,
                                    this->systemDimensionality,
                                    -2.0f, 2.0f);   
}

Smit::Smit() {
    this->setupSystem();
    this->initializePositions();
    this->initializeVelocities();
}

float Smit::potentialEnergy() {
    return this->potentialEnergy(this->positions);
}

float Smit::potentialEnergy(std::vector<std::vector<float>>& positions) {
    float totalPE = 0;
    for(int i = 0 ; i < this->numberOfParticles ; i++) {
        totalPE += this->U(positions[i][0]);
    }
    return totalPE;
}

std::vector<std::vector<float>> Smit::force() {
    std::vector<std::vector<float>> force_vector(
        this->numberOfParticles,
        std::vector<float>(this->systemDimensionality, 0)
    );

    for(int i = 0 ; i < this->numberOfParticles ; i++) {
        force_vector[i][0] = this->F(this->positions[i][0]);
    }
    return force_vector;
}

std::vector<std::vector<float>> Smit::force(std::vector<std::vector<float>>& positions) {
    std::vector<std::vector<float>> force_vector(
        this->numberOfParticles,
        std::vector<float>(this->systemDimensionality, 0)
    );

    for(int i = 0 ; i < this->numberOfParticles ; i++) {
        force_vector[i][0] = this->F(positions[i][0]);
    }
    return force_vector;
}

float Smit::U(float x) {
    if (x < -1.25)
        return 4 * pow(M_PI, 2) * pow((x + 1.25), 2);

    if (x < -0.25)
        return 2 * (1 + sin(2 * M_PI * x));

    if (x < 0.75)
        return 3 * (1 + sin(2 * M_PI * x));
    
    if (x < 1.75)
        return 4 * (1 + sin(2 * M_PI * x));
    
    return 8 * pow(M_PI, 2) * pow((x - 1.75), 2);
}

float Smit::F(float x) {
    if (x < -1.25)
        return -8 * pow(M_PI, 2) * (x + 1.25);

    if (x < -0.25)
        return -4 * M_PI * cos(2 * M_PI * x);

    if (x < 0.75)
        return -6 * M_PI * cos(2 * M_PI * x);
    
    if (x < 1.75)
        return -8 * M_PI * cos(2 * M_PI * x);
    
    return -16 * pow(M_PI, 2) * (x - 1.75);
}
