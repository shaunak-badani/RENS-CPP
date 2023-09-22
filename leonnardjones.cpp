#include "leonnardjones.h"
#include "system.h"
#include "math.h"

LeonnardJones::LeonnardJones() {
    this->setupSystem();
    this->initializePositions();
    this->initializeVelocities();
}

void LeonnardJones::setupSystem() {
    sigma = 3.4; // value in angstrom
    L = 10.229  * sigma / 2;
    kB = BOLTZ;
    epsilon = 120 * kB;
    arbitrary = false;

    // Won't be using the config N for this system
    this->numberOfParticles = 108;
    this->systemDimensionality = 3;
    for(int i = 0 ; i < this->numberOfParticles ; i++)
        this->masses.push_back(39.95f);
}

void LeonnardJones::initializePositions() {
    std::ifstream ljPositionsFile; 
    ljPositionsFile.open("LJ_config.txt");
    std::string positionValue;
    this->positions = std::vector<std::vector<float>>(this->numberOfParticles,
                std::vector<float>(this->systemDimensionality, 0.0f));
    
    // get positions from rst file
    for(int i = 0 ; i < this->numberOfParticles ; i++) {
        for(int j = 0 ; j < this->systemDimensionality ; j++) {
            ljPositionsFile >> positionValue;
            this->positions[i][j] = std::stof(positionValue);
        }
    }
}

void LeonnardJones::initializeVelocities() {
    float scale;
    float kT = kB * temperature() * kJ_mol_TO_J;
    for(int i = 0 ; i < this->numberOfParticles ; i++) {
        this->velocities.push_back(std::vector<float>(this->systemDimensionality, 0.0f));
        scale = sqrt(kT / (this->masses[i] * AMU_TO_KG)) * M_S_TO_A_PS;
        for(int j = 0 ; j < this->systemDimensionality ; j++) {
            this->velocities[i][j] = generateNormalRandom(0.0f, scale);
        }
    }
}

std::vector<std::vector<float>> LeonnardJones::force(std::vector<std::vector<float>>& pos) {
    int N = this->numberOfParticles;
    int d = this->systemDimensionality;
    std::vector<std::vector<float>> forceValue(this->numberOfParticles,
            std::vector<float>(this->systemDimensionality, 0));

    float eps = this->epsilon * kJ_mol_TO_J * pow(M_S_TO_A_PS, 2) / (AMU_TO_KG);
    
    std::vector<float> rIJ(this->systemDimensionality, 0);
    float dist;
    for(int i = 0 ; i < N ; i++) {
        for(int j = i + 1 ; j < N ; j++) {
            dist = 0;
            for(int k = 0 ; k < d ; k++) {
                
                rIJ[k] = (pos[i][k] - pos[j][k]);

                if(rIJ[k] >= this->L / 2)
                    rIJ[k] -= this->L;
                
                if(rIJ[k] <= -this->L / 2)
                    rIJ[k] += this->L;
                
                dist += pow(rIJ[k], 2);
            }
            dist = sqrt(dist);

            float exp_1 = pow(sigma / dist, 6);
            float exp_2 = exp_1 * exp_1;
            float value = (24 * eps) / (dist * dist) * (2 * exp_2 - exp_1);

            for(int k = 0 ; k < d ; k++) {
                forceValue[i][k] += value * rIJ[k];
                forceValue[j][k] -= value * rIJ[k];
            }
        }
    }

    return forceValue;
}

std::vector<std::vector<float>> LeonnardJones::force() {
    return force(this->positions);
}

float LeonnardJones::potentialEnergy(std::vector<std::vector<float>>& pos) {
    float peValue = 0;
    float dist;
    std::vector<float> rIJ(this->systemDimensionality, 0);

    for(int i = 0 ; i < this->numberOfParticles ; i++) {
        for(int j = i + 1 ; j < this->numberOfParticles ; j++) {
            dist = 0;
            for(int k = 0 ; k < this->systemDimensionality ; k++) {
                rIJ[k] = (pos[i][k] - pos[j][k]);

                if(rIJ[k] >= this->L / 2)
                    rIJ[k] -= this->L;
                
                if(rIJ[k] <= -this->L / 2)
                    rIJ[k] += this->L;

                dist += pow(rIJ[k], 2);
            }
            dist = sqrt(dist);

            float exp_1 = pow(sigma / dist, 6);
            float exp_2 = exp_1 * exp_1;

            peValue += 4 * epsilon * (exp_2 - exp_1);
        }
    }
    return peValue;
}

float LeonnardJones::potentialEnergy() {
    return this->potentialEnergy(this->positions);
}

float LeonnardJones::kineticEnergy() {
    return this->kineticEnergy(this->velocities);
}

float LeonnardJones::kineticEnergy(std::vector<std::vector<float>> velocities) {
    float mv2 = 0;
    for(int i = 0 ; i < this->numberOfParticles ; i++) {
        for(int j = 0 ; j < this->systemDimensionality ; j++) {
            mv2 += (this->masses[i] * AMU_TO_KG) * pow(velocities[i][j] * A_PS_TO_M_S, 2);
        }
    }
    return 0.5 * mv2 * NA / KILO;
} 

void LeonnardJones::systemConstraints() {
    for(int i = 0 ; i < this->numberOfParticles ; i++) {
        for(int j = 0 ; j < this->systemDimensionality ; j++) {
            if(this->positions[i][j] <= 0)
                this->positions[i][j] += L;
            
            if(this->positions[i][j] >= L)
                this->positions[i][j] -= L;
        }
    }
}

void LeonnardJones::handleOutput(float timeStep, FileOperations* fileOpObject) {
    fileOpObject->registerScalarData("KE", this->kineticEnergy());
    fileOpObject->registerScalarData("PE", this->potentialEnergy());
    fileOpObject->registerScalarData("TE", this->totalEnergy());
    fileOpObject->registerScalarData("T", this->instantaneousTemperature());
    fileOpObject->writeScalarData(timeStep);

    fileOpObject->registerVectorData("p", this->positions);
    fileOpObject->registerVectorData("v", this->velocities);
    fileOpObject->writeXYZfiles(timeStep);
}