#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <random>


std::vector<std::vector<float>> initializeRandomVector(int outerSize, int innerSize, float low = 0.0f, float high = 1.0f);

// want to generate numbers in range [a, b)
int generateUniformRandomInt(int low = 0, int high = 1);

float generateUniformRandom(float low = 0.0f, float high = 1.0f);

float generateNormalRandom(float mean = 0.0f, float stddev = 1.0f);

std::vector<std::vector<float>> generateNormalVector(int a, int b, float mean = 0.0f, float stddev = 1.0f);

#endif