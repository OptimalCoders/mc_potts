// Author:  Dominik Gresch <greschd@ethz.ch>
// Date:    16.04.2014 12:07:29 CEST
// File:    perf.cpp

#include <conf.hpp>
#include <../src/mc_potts.hpp>
#include <../src/addon/performance.hpp>

#include <iostream>

using namespace mc_potts;

int main(int argc, char* argv[]) {
    potts_model<LENGTH> sim(TEMP, 10, 2e6);
    //~ sim.thermalize();
    MEASURE_DIV(sim.update(), "", 2e6)
    P_RESULTS()
    return 0;
}

