#include "mullermod.h"

Muller::Muller() {
    this->setupSystem();
    this->initializePositions();
    this->initializeVelocities();
}

void Muller::setupSystem() {
    this->numberOfParticles = numParticles;
    this->systemDimensionality = 2;
    for(int i = 0 ; i < this->numberOfParticles ; i++)
        this->masses.push_back(1.0f);

    // Initializing constants
    A = {-200.0f, -100.0f, -170.0f, 15.0f};
    xMean = {1.0f, 0.0f, -0.5f, -1.0f};
    yMean = {0.0f, 0.5f, 1.5f, 1.0f};
    a = {-1.0f, -1.0f, -6.5f, 0.7f};
    b = {0.0f, 0.0f, 11.0f, 0.6f};
    c = {-10.0f, -10.0f, -6.5f, 0.7f};
}

void Muller::initializePositions() {
    for(int i = 0 ; i < this->numberOfParticles ; i++) {
        this->positions.push_back(std::vector<float>(this->systemDimensionality, 0.0f));
        this->positions[i][0] = generateUniformRandom(-0.525, -0.275);
        this->positions[i][1] = generateUniformRandom(1.5, 1.8);
    }
}

std::vector<std::vector<float>> Muller::force(std::vector<std::vector<float>>& pos) {
    std::vector<std::vector<float>> returnValue;
    for(int i = 0 ; i < this->numberOfParticles ; i++) 
        returnValue.push_back(F(pos[i]));                 
    return returnValue;
}

std::vector<std::vector<float>> Muller::force() {
    return this->force(this->positions);
}

float Muller::potentialEnergy(std::vector<std::vector<float>>& pos) {
    float peValue = 0;
    for(int i = 0 ; i < this->numberOfParticles ; i++) 
        peValue += U(pos[i]);                 
    return peValue;
}

float Muller::potentialEnergy() {
    return this->potentialEnergy(this->positions);
}

float Muller::U(std::vector<float> point) {
    float x = point[0];
    float y = point[1];

    float peValue = 0;
    float powValue;
    for(int i = 0 ; i < 4 ; i++) {
        powValue = a[i] * pow(x - xMean[i], 2) + b[i] * (x - xMean[i]) * (y - yMean[i]) + c[i] * pow(y - yMean[i], 2);
        peValue += A[i] * exp(powValue);
    }
    return peValue;
}

std::vector<float> Muller::F(std::vector<float> point) {
    float x = point[0];
    float y = point[1];

    float fX, fY;
    fX = fY = 0;

    float peValue = 0;
    float powValue;
    for(int i = 0 ; i < 4 ; i++) {
        powValue = a[i] * pow(x - xMean[i], 2) + b[i] * (x - xMean[i]) * (y - yMean[i]) + c[i] * pow(y - yMean[i], 2);

        fX += A[i] * exp(powValue) * (- 2 * a[i] * (x - xMean[i]) - b[i] * (y - yMean[i]));
        fY += A[i] * exp(powValue) * (-b[i] * (x - xMean[i]) - 2 * c[i] * (y - yMean[i]));
    }
    return {fX, fY};
}

// Muller Mod

MullerMod::MullerMod() {
    this->sigma = 0.8f;
    this->sigma_2 = pow(sigma, 2);
    this->height = 200.0f;
    this->center = {-0.25f, 0.65f};
}

/* Need the following functions, but are duplicate of the ones in Muller
they don't call the protected function of MullerMod
*/

std::vector<std::vector<float>> MullerMod::force(std::vector<std::vector<float>>& pos) {
    std::vector<std::vector<float>> returnValue;
    for(int i = 0 ; i < this->numberOfParticles ; i++) 
        returnValue.push_back(F(pos[i]));                 
    return returnValue;
}

std::vector<std::vector<float>> MullerMod::force() {
    return this->force(this->positions);
}

float MullerMod::potentialEnergy(std::vector<std::vector<float>>& pos) {
    float peValue = 0;
    for(int i = 0 ; i < this->numberOfParticles ; i++) 
        peValue += U(pos[i]);                 
    return peValue;
}

float MullerMod::potentialEnergy() {
    return this->potentialEnergy(this->positions);
}

float MullerMod::U(std::vector<float> pos) {

    float peValue = Muller::U(pos);

    float x = pos[0];
    float y = pos[1];
    float powValue = pow(x - center[0], 2) + pow(y - center[1], 2);
    powValue /= (2 * sigma_2);

    float extraTerm = height * exp(-powValue);
    return peValue + extraTerm;
}

std::vector<float> MullerMod::F(std::vector<float> pos) {
    std::vector<float> fValue = Muller::F(pos);

    float x = pos[0];
    float y = pos[1];
    float powValue = pow(x - center[0], 2) + pow(y - center[1], 2);
    powValue /= (2 * sigma_2);

    float amplitude = height * exp(-powValue);

    fValue[0] += amplitude * (x - center[0]) / (sigma_2);
    fValue[1] += amplitude * (y - center[1]) / (sigma_2);

    return fValue;
}