#include <iostream>
#include <cstdlib>
#include <vector>

std::vector<std::vector<float>> initializeRandomVector(int outerSize, int innerSize, float low = 0.0f, float high = 1.0f) {
    std::vector<std::vector<float>> returnValue(outerSize, std::vector<float>(innerSize, innerSize));

    for(int i = 0 ; i < outerSize ; i++) {
        for(int j = 0 ; j < innerSize ; j++) {
            returnValue[i][j] = low + (high - low) * (float) rand() / RAND_MAX;
        }
    }
    return returnValue;
}