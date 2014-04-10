// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    02.04.2014 21:13:33 CEST
// File:    msk_version_comparison.cpp

#include <alpha.hpp>
#include <validation.hpp>
#include <addon/performance.hpp>

#include <iostream>


int main(int argc, char* argv[]) {
    using namespace mc_potts;
    
    #define rng_type addon::lag_fib_rng
    //~ #define rng_type addon::std_mt_rng
    double T = 1;
    int const L = 32;
    
    //~ validate<baseline_mskoenz::sim, msk_v1::sim, 4, rng_type>();
    
    addon::detail::lag_fib_engine.seed(0);
    baseline_mskoenz::sim::impl<L, L, L, rng_type> s1(T, 10);
    s1.thermalize();
    MEASURE_DIV(s1.update(), "baseline_msk", L*L*L)
    
    addon::detail::lag_fib_engine.seed(0);
    baseline_greschd::sim::impl<L, L, L, rng_type> s2(T, 10);
    s2.thermalize();
    MEASURE_DIV(s2.update(), "baseline_dag", L*L*L)
    
    addon::detail::lag_fib_engine.seed(0);
    msk_v1::sim::impl<L, L, L, rng_type> s3(T, 10);
    s3.thermalize();
    MEASURE_DIV(s3.update(), "msk_v01", L*L*L)
    
    addon::detail::lag_fib_engine.seed(0);
    v1_int2t::sim::impl<L, L, L, rng_type> s4(T, 10);
    s4.thermalize();
    MEASURE_DIV(s4.update(), "int2t", L*L*L)
    
    addon::detail::lag_fib_engine.seed(0);
    dag_v02::sim::impl<L, L, L, rng_type> s5(T, 10);
    s5.thermalize();
    MEASURE_DIV(s5.update(), "int2t-zorder", L*L*L)
    
    P_RESULTS()
    P_SPEEDUP()
    
    CLEAR_MEASURE()
    
    return 0;
}
