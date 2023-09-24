#include "utilities.h"

#include <iostream>
#include <cstdlib>
#include <vector>
#include <random>

std::vector<std::vector<float>> initializeRandomVector(int outerSize, int innerSize, float low, float high) {
    std::vector<std::vector<float>> returnValue(outerSize, std::vector<float>(innerSize, innerSize));

    for(int i = 0 ; i < outerSize ; i++) {
        for(int j = 0 ; j < innerSize ; j++) {
            returnValue[i][j] = low + (high - low) * ((float) rand() / RAND_MAX);
        }
    }
    return returnValue;
}

std::default_random_engine generator;

// want to generate numbers in range [a, b)
int generateUniformRandomInt(int low, int high) {
    // std::random_device rd;
    // std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(low, high - 1);
    return dis(generator);
}

float generateUniformRandom(float low, float high) {
    // std::random_device rd;
    // std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(low, high);
    return dis(generator);
}

float generateNormalRandom(float mean, float stddev) {
    std::normal_distribution<float> distribution(mean, stddev);
    return distribution(generator);
}

std::vector<std::vector<float>> generateNormalVector(int a, int b, float mean, float stddev) {
    std::vector<std::vector<float>> normalVector(a, std::vector<float>(b, 0.0f));
    for(int i = 0 ; i < a ; i++) 
        for(int j = 0 ; j < b ; j++)
            normalVector[i][j] = generateNormalRandom(mean, stddev);
        
    return normalVector;
}