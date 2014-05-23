// Author:  Dominik Gresch <greschd@ethz.ch>
// Date:    20.05.2014 13:29:42 CEST
// File:    perf_pcm_mem.cpp

#include <conf.hpp>
#include <../src/mc_potts.hpp>

#include <../pcm/measuring_core.h>

#include <iostream>

using namespace mc_potts;


int main(int argc, char* argv[]) {
    potts_model<LENGTH> sim(TEMP, 10, 2e6);
    
    sim.thermalize();
    
    uint64_t runs = 1;
    
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
