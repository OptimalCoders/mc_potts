// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    02.04.2014 21:13:33 CEST
// File:    msk_version_comparision.cpp

#include <validation.hpp>
#include <msk_v01/sim.hpp>
#include <rng/lag_fib_rng.hpp>
#include <rng/std_mt_rng.hpp>
#include <addon/performance.hpp>
#include <baseline_impl_mskoenz/sim.hpp>
#include <baseline_impl_greschd/sim.hpp>

#include <iostream>


int main(int argc, char* argv[]) {
    using namespace mc_potts;
    
    #define rng_type addon::lag_fib_rng
    //~ #define rng_type addon::std_mt_rng
    double T = 1;
    int const L = 128;
    
    validate<baseline_mskoenz::sim, msk_v1::sim, 4, rng_type>();
    
    addon::detail::lag_fib_engine.seed(0);
    baseline_mskoenz::sim::impl<L, L, L, 4, rng_type> s1(T, 10);
    s1.thermalize();
    MEASURE_DIV(s1.update(), "baseline_msk", L*L*L)
    
    addon::detail::lag_fib_engine.seed(0);
    baseline_greschd::sim::impl<L, L, L, 4, rng_type> s2(T, 10);
    s2.thermalize();
    MEASURE_DIV(s2.update(), "baseline_dag", L*L*L)
    
    addon::detail::lag_fib_engine.seed(0);
    msk_v1::sim::impl<L, L, L, 4, rng_type> s3(T, 10);
    s3.thermalize();
    MEASURE_DIV(s3.update(), "msk_v01", L*L*L)
    
    P_RESULTS()
    P_SPEEDUP()
    
    CLEAR_MEASURE()
    
    return 0;
}
