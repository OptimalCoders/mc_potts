// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    28.04.2014 23:38:30 CEST
// File:    perf_screen.cpp

#include <conf.hpp>
#include <../src/beta.hpp>
#include <../src/addon/performance.hpp>

#include <iostream>

using namespace mc_potts;

int main(int argc, char* argv[]) {
    int const N = 100;
    typename SIM_MACRO::template impl<LENGTH, LENGTH, LENGTH, addon::RNG_MACRO, GRID_MACRO, MATRIX_MACRO> sim(TEMP, 10, N);
    sim.thermalize();
    MEASURE_DIV(sim.update(), "", N)
    P_RESULTS()
    return 0;
}

