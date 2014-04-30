// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    26.03.2014 15:45:36 CET
// File:    performance_example.cpp

#include <iostream>
#include <beta.hpp>
#include <alpha.hpp>
#include <addon/performance.hpp>

int main(int argc, char* argv[]) {
    int up = 1<<1;
    addon::std_mt_rng<int> rng1(0, up);
    addon::lag_fib_rng<int> rng2(0, up);
    addon::custom_mt_rng<int> rng3(0, up);
    addon::eff_int_mt_rng<int> rng4(0, up);
    
    // MEASURE takes an expression and a name
    MEASURE(rng1(), rng1.name())
    MEASURE(rng2(), rng2.name())
    MEASURE(rng3(), rng3.name())
    MEASURE(rng4(), rng4.name())
    
    // P_RESULTS just prints all results in a nice way
    P_RESULTS()
    
    // CLEAR_MEASURE() clears all measurements
    CLEAR_MEASURE()
    
    // measure rng again
    //~ MEASURE(rng1(), "std_mt_rng")
    //~ MEASURE(rng2(), "lag_fib_rng")
    MEASURE(rng3(), "custom_mt_rng")
    MEASURE(rng4(), "eff_int_mt_rng")
    
    // P_SPEEDUP prints the speedup (x) relative to the slowest measurement (decreasing order)
    P_SPEEDUP()
    
    return 0;
}
