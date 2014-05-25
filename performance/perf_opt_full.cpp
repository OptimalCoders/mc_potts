// Author:  Dominik Gresch <greschd@ethz.ch>
// Date:    16.04.2014 12:07:29 CEST
// File:    perf.cpp

#include <conf.hpp>
#include <../src/mc_potts_full.hpp>
#include <../src/addon/performance.hpp>

#include <iostream>

using namespace mc_potts;

int main(int argc, char* argv[]) {
    const int N = 50;
    potts_model<LENGTH> sim(TEMP, 10, N);
    sim.thermalize();
    MEASURE_DIV(sim.update(), "", N)
    P_RESULTS()
    return 0;
}

