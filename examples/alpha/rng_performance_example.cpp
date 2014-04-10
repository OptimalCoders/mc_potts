// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    26.03.2014 15:45:36 CET
// File:    performance_example.cpp

#include <iostream>
#include <rng/std_mt_rng.hpp>
#include <rng/lag_fib_rng.hpp>
#include <rng/custom_mt_rng.hpp>
#include <addon/performance.hpp>

int main(int argc, char* argv[]) {
    addon::std_mt_rng<int> rng1;
    addon::lag_fib_rng<int> rng2;
    addon::custom_mt_rng<int> rng3;
    
    // MEASURE takes an expression and a name
    MEASURE(rng1(), rng1.name())
    MEASURE(rng2(), rng2.name())
    MEASURE(rng3(), rng3.name())
    
    // P_RESULTS just prints all results in a nice way
    P_RESULTS()
    
    // CLEAR_MEASURE() clears all measurements
    CLEAR_MEASURE()
    
    // measure rng again
    MEASURE(rng1(), "std_mt_rng")
    MEASURE(rng2(), "lag_fib_rng")
    MEASURE(rng3(), "custom_mt_rng")
    
    // P_SPEEDUP prints the speedup (x) relative to the slowest measurement (decreasing order)
    P_SPEEDUP()
    
    return 0;
}
