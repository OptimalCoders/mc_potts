// Author:  Dominik Gresch <greschd@ethz.ch>
// Date:    20.05.2014 13:29:42 CEST
// File:    perf_pcm_mem.cpp

#include <conf.hpp>
#include <../src/beta.hpp>
#include <../src/addon/performance.hpp>

#include <../pcm/measuring_core.h>

#include <iostream>

using namespace mc_potts;


int main(int argc, char* argv[]) {
    baseline_greschd_sim::impl<LENGTH, LENGTH, LENGTH, addon::std_mt_rng, baseline_greschd_grid, baseline_greschd_matrix> sim(10, 10);
    
    sim.thermalize();
    
    //~ uint64_t runs = 1e6/(LENGTH * LENGTH * LENGTH) + 1;
    uint runs = 2;
    long counters[] {0x10, 0x01, 0x10, 0x80, 0x10, 0x10, 0x11, 0x02};
    
    measurement_init(counters, 0, 0);
     
    measurement_start();
        
    for(uint i = 0; i < runs; ++i) {
        sim.update();
    }
    
    measurement_stop(runs);
    measurement_end();
    
    return 0;
}
