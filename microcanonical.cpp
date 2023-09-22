#include "microcanonical.h"

MicroCanonical::MicroCanonical() {
    this->dt = 0.001;
}

void MicroCanonical::step(System* sys, FileOperations* fileOpObject, int numSteps) {
    for(int n = 0 ; n < numSteps ; n++) {
        std::vector<std::vector<float>> force = sys->force();
        for(int i = 0 ; i < sys->numberOfParticles ; i++) {
            for(int j = 0 ; j < sys->systemDimensionality ; j++) {
                sys->velocities[i][j] += this->dt * (force[i][j] / (2 * (sys->masses[i])));
                sys->positions[i][j] += this->dt * (sys->velocities[i][j]);
            }
        }

        force = sys->force();
        for(int i = 0 ; i < sys->numberOfParticles ; i++) {
            for(int j = 0 ; j < sys->systemDimensionality ; j++)
                sys->velocities[i][j] += this->dt * (force[i][j] / (2 * (sys->masses[i])));
        }

        sys->systemConstraints();

        if(n % outputPeriod == 0) {
            sys->handleOutput((float)(n * this->dt), fileOpObject);
        }
    }
}