// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    28.04.2014 23:38:30 CEST
// File:    perf_screen.cpp

//~ #define NO_COLOR
//~ #define ENABLE_MICRO

#include <conf.hpp>
#include <../src/beta.hpp>
#include <../src/addon/performance.hpp>

#include <iostream>

using namespace mc_potts;

int main(int argc, char* argv[]) {
    int const N = 100;
    typename SIM_MACRO::template impl<LENGTH, LENGTH, LENGTH, addon::RNG_MACRO, GRID_MACRO, MATRIX_MACRO> sim(TEMP, 10, N);
    sim.thermalize();
    
    for(uint i = 0; i < 1000; ++i) {
        START_MICRO("main")
        sim.update();
        STOP_MICRO()
    }
    auto res = RES_MICRO("step");
    
    for(auto const & a : res)
        std::cout << a.first << " " << a.second.first << " " << a.second.second << std::endl;
    
    return 0;
}

