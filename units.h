#ifndef UNITS_H
#define UNITS_H

float kB = 1;
bool arbitrary = true;

// FOR LJ system
float BOLTZMANN = 1.380649e-23;
float NA = 6.023e23;
float KILO = 1e3;
float RGAS = BOLTZMANN * NA;
float BOLTZ = RGAS / KILO;

float AMU_TO_KG = 1 / (KILO * NA);
float A_PS_TO_M_S = 1e2;
float M_S_TO_A_PS = 1 / (A_PS_TO_M_S);
float kJ_mol_TO_J = KILO / NA;
float S_TO_PS = 1e-12;

#endif