// Author:  Dominik Gresch <greschd@ethz.ch>
// Date:    20.05.2014 13:29:42 CEST
// File:    perf_pcm_mem.cpp

#include <conf.hpp>
#include <../src/beta.hpp>
#include <../src/addon/performance.hpp>

#include <../pcm/MeasuringCore.lib>

#include <iostream>

using namespace mc_potts;


int main(int argc, char* argv[]) {
    baseline_greschd_sim::impl<10, 10, 10, addon::std_mt_rng, baseline_greschd_grid, baseline_greschd_matrix> sim(10, 10);
    
    sim.thermalize();
    
    constexpr int runs = 1;
    long * counters = [];
    
    measurement_init(counters, 0, 0);
     
    measurement_start();
        
    for(uint i = 0; i < runs; ++i) {
        sim.update();
    }
    
    measurement_stop(runs);
    measurement_end();
    
    return 0;
}

//~ int main(int argc, char* argv[]) {
    //~ typename SIM_MACRO::template impl<LENGTH, LENGTH, LENGTH, addon::RNG_MACRO, GRID_MACRO, MATRIX_MACRO> sim(TEMP, 10);
    //~ sim.thermalize();
    //~ sim.update();        
    //~ return 0;
//~ }
