#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <random>

std::vector<std::vector<float>> initializeRandomVector(int outerSize, int innerSize, float low = 0.0f, float high = 1.0f) {
    std::vector<std::vector<float>> returnValue(outerSize, std::vector<float>(innerSize, innerSize));

    for(int i = 0 ; i < outerSize ; i++) {
        for(int j = 0 ; j < innerSize ; j++) {
            returnValue[i][j] = low + (high - low) * ((float) rand() / RAND_MAX);
        }
    }
    return returnValue;
}

float generateUniformRandom(float low = 0.0f, float high = 1.0f) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(low, high);
    return dis(gen);
}

float generateNormalRandom(float mean = 0.0f, float stddev = 1.0f) {
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(mean, stddev);
    return (float) distribution(generator);
}

#endif