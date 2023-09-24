#include "smit.h"
#include "system.h"


int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);

    System* sys = new Smit();
    std::cout << "Potential energy: " << sys->potentialEnergy() << std::endl;
    return 0;

    MPI_Finalize();
}