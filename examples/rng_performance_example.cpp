// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    26.03.2014 15:45:36 CET
// File:    performance_example.cpp

#include <iostream>
#include <addon/performance.hpp>
#include <rng/std_mt_rng.hpp>
#include <rng/lag_fib_rng.hpp>
#include <baseline_impl_greschd/sim.hpp>
#include <baseline_impl_mskoenz/sim.hpp>

int main(int argc, char* argv[]) {
    
    mc_potts::baseline_greschd::sim::impl<100, 100, 100, 4, addon::std_mt_rng> sim1(100, 1.1);
    mc_potts::baseline_mskoenz::sim::impl<100, 100, 100, 4, addon::lag_fib_rng> sim2(100, 1.1);
    
    addon::std_mt_rng<int> rng1;
    addon::lag_fib_rng<int> rng2;
    
    const int N = 1;
    
    addon::clock.start();
    for(uint i = 0; i < 10*N; ++i) {
        sim1.update();
        //~ rng1();
    }
    addon::clock.stop();
    std::cout << addon::clock.cycles() << std::endl;
    
    addon::clock.start();
    for(uint i = 0; i < 10*N; ++i) {
        sim2.update();
        //~ rng2();
    }
    addon::clock.stop();
    std::cout << addon::clock.cycles() << std::endl;
    return 0;
}
