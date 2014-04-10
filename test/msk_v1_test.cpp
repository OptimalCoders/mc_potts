// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    09.04.2014 17:10:29 CEST
// File:    msk_v1_test.cpp

#include <beta.hpp>
#include <alpha.hpp>
#include <iostream>

#include <validation.hpp>
#include <addon/performance.hpp>

int main(int argc, char* argv[]) {
    using namespace mc_potts;
    int const L = 128;
    
    int const up = 1000;
    
    msk_v1_sim::impl<L, L, L, addon::lag_fib_rng
                            , msk_v1_pbc
                            , msk_v1_zorder
                            > s1(10, 10, up);
    
    msk_v1_sim::impl<L, L, L, addon::lag_fib_rng
                            , msk_v1_pbc
                            , msk_v0_std_vec
                            > s2(10, 10, up);
    
    msk_v1_sim::impl<L, L, L, addon::lag_fib_rng
                            , msk_v1_pbc
                            , msk_v0_c_array_static
                            > s3(10, 10, up);

    msk_v1_sim::impl<L, L, L, addon::lag_fib_rng
                            , msk_v1_pbc
                            , msk_v0_c_array_dynamic
                            > s4(10, 10, up);
    
    baseline_mskoenz::sim::impl<L, L, L, addon::std_mt_rng> s5(10, 10, up);
    
    baseline_greschd_sim::impl<L, L, L, addon::lag_fib_rng
                                      , baseline_greschd_grid
                                      , int2t_v01_matrix> s6(10, 10, up);
    
    msk_v1_sim::impl<L, L, L, addon::custom_mt_rng
                            , msk_v1_pbc
                            , msk_v2_static_zip
                            > s7(10, 10, up);
    
    validate<msk_v1_sim, msk_v1_sim, addon::lag_fib_rng, msk_v1_pbc, msk_v1_pbc, msk_v0_c_array_static, msk_v2_static_zip>();
    
    //~ MEASURE_DIV(s1.update(), s1.spec(), up)
    //~ MEASURE_DIV(s2.update(), s2.spec(), up)
    MEASURE_DIV(s3.update(), s3.spec(), up)
    //~ MEASURE_DIV(s4.update(), s4.spec(), up)
    MEASURE_DIV(s5.update(), s5.spec(), up)
    //~ MEASURE_DIV(s6.update(), s6.spec(), up)
    MEASURE_DIV(s7.update(), s7.spec(), up)
    
    P_RESULTS()
    P_SPEEDUP()
    
    return 0;
}
