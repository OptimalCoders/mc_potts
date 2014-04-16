// Author:  Dominik Gresch <greschd@ethz.ch>
// Date:    16.04.2014 12:07:29 CEST
// File:    perf.cpp

#include <conf.hpp>
#include <../src/beta.hpp>
#include <../src/addon/performance.hpp>

#include <iostream>

using namespace mc_potts;

int main(int argc, char* argv[]) {
    typename SIM_MACRO::template impl<LENGTH, LENGTH, LENGTH, addon::RNG_MACRO, GRID_MACRO, MATRIX_MACRO> sim(TEMP, 10);
    sim.thermalize();
    MEASURE_DIV(sim.update(), "", LENGTH*LENGTH*LENGTH)
    P_RESULTS()
    return 0;
}

