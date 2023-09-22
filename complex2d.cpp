#include "complex2d.h"
#include "utilities.h"

Complex2D::Complex2D() 
{
    this->setupSystem();
    this->initializePositions();
    this->initializeVelocities();
}

void Complex2D::setupSystem() {
    this->numberOfParticles = numParticles;
    this->systemDimensionality = 2;
    for(int i = 0 ; i < this->numberOfParticles ; i++)
        this->masses.push_back(1.0f);
}

void Complex2D::initializePositions() {
    this->positions = initializeRandomVector(this->numberOfParticles,
                                    this->systemDimensionality,
                                    -1.0f, 1.0f);
}

float Complex2D::potentialEnergy(std::vector<std::vector<float>>& pos) {
    float peValue = 0;
    for(int i = 0 ; i < this->numberOfParticles ; i++) 
        peValue += U(pos[i]);                 
    return peValue;
}

float Complex2D::potentialEnergy() {
    return potentialEnergy(this->positions);
}


std::vector<std::vector<float>> Complex2D::force(std::vector<std::vector<float>>& pos) {
    std::vector<std::vector<float>> returnValue;
    for(int i = 0 ; i < this->numberOfParticles ; i++) 
        returnValue.push_back(F(pos[i]));                 
    return returnValue;
}

std::vector<std::vector<float>> Complex2D::force() {
    return this->force(this->positions);
}

float Complex2D::U(std::vector<float>& pos) {
    float x = pos[0];
    float y = pos[1];

    return 10 * (pow(x, 10) + pow(y, 10)) + 5 * sin(2 * M_PI * x) * cos(2 * M_PI * y);
}

std::vector<float> Complex2D::F(std::vector<float>& pos) {
    float x = pos[0];
    float y = pos[1];

    float fX = -1 * (100 * pow(x, 9) + 10 * M_PI * cos(2 * M_PI * x) * cos(2 * M_PI * y));            
    float fY = -1 * (100 * pow(y, 9) - 10 * M_PI * sin(2 * M_PI * x) * sin(2 * M_PI * y));

    return {fX, fY};
}
