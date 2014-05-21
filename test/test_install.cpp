// Author:  Dominik Gresch <greschd@ethz.ch>
// Date:    20.05.2014 23:31:26 CEST
// File:    test_install.cpp

#include <../src/mc_potts.hpp>

#include <iostream>

using namespace mc_potts;
int main(int argc, char* argv[]) {
    potts_model<10> sim(10, 10);
    sim.thermalize();
    sim.update();
    return 0;
}

