// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    09.04.2014 17:10:29 CEST
// File:    msk_v1_test.cpp

#include <beta.hpp>
#include <alpha.hpp>
#include <iostream>

#include <validation.hpp>
#include <addon/performance.hpp>

int main(int argc, char* argv[]) {
    
    int const L = 128;
    
    mc_potts::msk_v1_sim::impl<L, L, L, addon::lag_fib_rng
                                      , mc_potts::msk_v1_pbc
                                      , mc_potts::msk_v1_zorder
                                      > s1(10, 10);
    
    mc_potts::msk_v1_sim::impl<L, L, L, addon::lag_fib_rng
                                      , mc_potts::msk_v1_pbc
                                      , mc_potts::msk_v0_std_vec
                                      > s2(10, 10);
    
    mc_potts::msk_v1_sim::impl<L, L, L, addon::lag_fib_rng
                                      , mc_potts::msk_v1_pbc
                                      , mc_potts::msk_v0_c_array_static
                                      > s3(10, 10);
    
    mc_potts::msk_v1_sim::impl<L, L, L, addon::lag_fib_rng
                                      , mc_potts::msk_v1_pbc
                                      , mc_potts::msk_v0_c_array_dynamic
                                      > s4(10, 10);
    
    mc_potts::baseline_mskoenz::sim::impl<L, L, L, addon::lag_fib_rng> s5(10, 10);
    
    MEASURE_DIV(s1.update(), s1.spec(), L*L*L)
    MEASURE_DIV(s2.update(), s2.spec(), L*L*L)
    MEASURE_DIV(s3.update(), s3.spec(), L*L*L)
    MEASURE_DIV(s4.update(), s4.spec(), L*L*L)
    MEASURE_DIV(s5.update(), s5.spec(), L*L*L)
    
    P_RESULTS()
    P_SPEEDUP()
    
    return 0;
}
