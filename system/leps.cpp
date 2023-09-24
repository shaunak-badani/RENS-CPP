// LEPS - I
#include "leps.h"


LEPS::LEPS() {
    this->setupSystem();
    this->initializePositions();
    this->initializeVelocities();
}

void LEPS::setupSystem() {
    this->numberOfParticles = numParticles;
    this->systemDimensionality = 2;
    for(int i = 0 ; i < this->numberOfParticles ; i++)
        this->masses.push_back(1.0f);

    a = 0.05;
    b = 0.3;
    c = 0.05;
    dAB = 4.746;
    dBC = 4.746;
    dAC = 3.445;
    alpha = 1.942;
    r0 = 0.742;
}

void LEPS::initializePositions() {
    for(int i = 0 ; i < this->numberOfParticles ; i++) {
        this->positions.push_back(std::vector<float>(this->systemDimensionality, 0.0f));
        this->positions[i][0] = generateUniformRandom(0.5, 1.0);
        this->positions[i][1] = generateUniformRandom(0.0, 1.0);
    }
}

std::vector<std::vector<float>> LEPS::force(std::vector<std::vector<float>>& pos) {
    std::vector<std::vector<float>> returnValue;
    for(int i = 0 ; i < this->numberOfParticles ; i++) 
        returnValue.push_back(F(pos[i]));                 
    return returnValue;
}

std::vector<std::vector<float>> LEPS::force() {
    return this->force(this->positions);
}

float LEPS::potentialEnergy(std::vector<std::vector<float>>& pos) {
    float peValue = 0;
    for(int i = 0 ; i < this->numberOfParticles ; i++) 
        peValue += U(pos[i]);                 
    return peValue;
}

float LEPS::potentialEnergy() {
    return this->potentialEnergy(this->positions);
}


float LEPS::Q(float r, float d) {
    float rV = 1.5 * exp(-2 * alpha * (r - r0));
    rV -= exp(-alpha * (r - r0));
    return (d / 2) * rV;
}

float LEPS::Q_der(float r, float d) {
    float rV = 3 * exp(-2 * alpha * (r - r0));
    rV -= exp(-alpha * (r - r0));
    return (-d * alpha / 2) * rV;
}

float LEPS::J(float r, float d) {
    float rV = exp(-2 * alpha * (r - r0));
    rV -= 6 * exp(-alpha * (r - r0));
    return (d / 4) * rV;
}

float LEPS::J_der(float r, float d) {
    float rV = exp(-2 * alpha * (r - r0));
    rV -= 3 * exp(-alpha * (r - r0));
    return (-d * alpha / 2) * rV;
}

float LEPS::U(std::vector<float> point) {
    float rAB = point[0];
    float rBC = point[1];
    float rAC = rAB + rBC;

    float rv = Q(rAB, dAB) / (1 + a);
    rv += Q(rBC, dBC) / (1 + b);
    rv += Q(rAC, dAC) / (1 + c);

    float jAB = J(rAB, dAB);
    float jBC = J(rBC, dBC);
    float jAC = J(rAC, dAC);

    float remTerm = pow(jAB / (1 + a), 2);
    remTerm += pow(jBC / (1 + b), 2);
    remTerm += pow(jAC / (1 + c), 2);
    remTerm -= jAB * jBC / ((1 + a) * (1 + b));
    remTerm -= jBC * jAC / ((1 + b) * (1 + c));
    remTerm -= jAB * jAC / ((1 + a) * (1 + c));
    remTerm = sqrt(remTerm);

    return rv - remTerm;
}

std::vector<float> LEPS::F(std::vector<float> point) {
    float rAB = point[0];
    float rBC = point[1];
    float rAC = rAB + rBC;

    float jAB = J(rAB, dAB);
    float jBC = J(rBC, dBC);
    float jAC = J(rAC, dAC);

    float qAB_der = Q_der(rAB, dAB);
    float qBC_der = Q_der(rBC, dBC);
    float qAC_der = Q_der(rAC, dAC);

    float jAB_der = J_der(rAB, dAB);
    float jBC_der = J_der(rBC, dBC);
    float jAC_der = J_der(rAC, dAC);

    float jValues = pow(jAB / (1 + a), 2) + pow(jBC / (1 + b), 2) + pow(jAC / (1 + c), 2);
    jValues -= (jAB * jBC) / ((1 + a) * (1 + b));
    jValues -= (jBC * jAC) / ((1 + b) * (1 + c));
    jValues -= (jAB * jAC) / ((1 + a) * (1 + c));


    // Computing F_x
    float f_x = qAB_der / (1 + a);
    f_x += qAC_der / (1 + c);

    float compX = (2 * jAB * jAB_der) / pow(1 + a, 2) + (2 * jAC * jAC_der) / pow(1 + c, 2);
    compX -= (jAB_der * jBC) / ((1 + a) * (1 + b)) + (jBC * jAC_der) / ((1 + b) * (1 + c));
    compX -= (jAB_der * jAC + jAC_der * jAB) / ((1 + a) * (1 + c));

    compX *= 1 / (2 * sqrt(jValues));
    f_x -= compX;

    // Computing F_y
    float f_y = qBC_der / (1 + b);
    f_y += qAC_der / (1 + c);

    float compY = (2 * jBC * jBC_der) / pow(1 + b, 2) + (2 * jAC * jAC_der) / pow(1 + c, 2);
    compY -= ((jAB * jBC_der) / ((1 + a) * (1 + b)) + (jAB * jAC_der) / ((1 + a) * (1 + c)));
    compY -= ((jBC_der * jAC + jBC * jAC_der) / ((1 + b) * (1 + c)));
    compY *= 1 / (2 * sqrt(jValues));

    f_y -= compY;

    return {-f_x, -f_y};
}

// LEPS - II

LEPS2::LEPS2() {
    kC = 0.2025;
    c = 1.154;
    rAC = 3.742;
    b = 0.03;
}

std::vector<std::vector<float>> LEPS2::force(std::vector<std::vector<float>>& pos) {
    std::vector<std::vector<float>> returnValue;
    for(int i = 0 ; i < this->numberOfParticles ; i++) 
        returnValue.push_back(F(pos[i]));                 
    return returnValue;
}

std::vector<std::vector<float>> LEPS2::force() {
    return this->force(this->positions);
}

float LEPS2::potentialEnergy(std::vector<std::vector<float>>& pos) {
    float peValue = 0;
    for(int i = 0 ; i < this->numberOfParticles ; i++) 
        peValue += U(pos[i]);                 
    return peValue;
}

float LEPS2::potentialEnergy() {
    return this->potentialEnergy(this->positions);
}

float LEPS2::U(std::vector<float> point) {
    float rAB = point[0];
    float x = point[1];

    float l = LEPS::U({rAB, rAC - rAB});
    return l + 2 * kC * pow(rAB - (rAC / 2 - x / c), 2);
}

std::vector<float> LEPS2::F(std::vector<float> point) {
    float rAB = point[0];
    float x = point[1];

    std::vector<float> f_i = LEPS::F({rAB, rAC - rAB});
    float f_x = f_i[0] - f_i[1] - 4 * kC * (rAB - (rAC / 2 - x / c));

    float f_y = -4 * (kC / c) * (rAB - (rAC / 2 - x / c));
    return {f_x, f_y};
}