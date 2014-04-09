// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    26.03.2014 15:45:36 CET
// File:    performance_example.cpp

#include <iostream>
#include <rng/std_mt_rng.hpp>
#include <rng/lag_fib_rng.hpp>
#include <addon/performance.hpp>
#include <alpha/baseline_impl_greschd/sim.hpp>
#include <alpha/baseline_impl_mskoenz/sim.hpp>

int main(int argc, char* argv[]) {
    
    uint32_t const L = 100;
    
    mc_potts::baseline_greschd::sim::impl<L, L, L, addon::std_mt_rng> sim1(100, 1.1);
    mc_potts::baseline_mskoenz::sim::impl<L, L, L, addon::std_mt_rng> sim2(100, 1.1);
    
    sim1.thermalize();
    sim2.thermalize();
    
    // MEASURE_DIV does the same as MEASURE but takes another parameter. 
    // The measured cycles will be devided by ths parameter
    MEASURE_DIV(sim1.update(), "baseline_greschd", L*L*L)
    MEASURE_DIV(sim2.update(), "baseline_mskoenz", L*L*L)
    
    // P_RESULTS just prints all results in a nice way
    P_RESULTS()
    
    // P_SPEEDUP prints the speedup (x) relative to the slowest measurement (decreasing order)
    P_SPEEDUP()
    
    return 0;
}
