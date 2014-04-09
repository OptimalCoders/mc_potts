// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    26.03.2014 10:11:58 CET
// File:    test_validation.cpp

#include <rng/std_mt_rng.hpp>
#include <rng/lag_fib_rng.hpp>
#include <baseline_impl_greschd/sim.hpp>
#include <v1_int2t/sim.hpp>
#include <baseline_impl_mskoenz/sim.hpp>
#include <validation.hpp>

#include <iostream>

using namespace mc_potts;


int main(int argc, char* argv[]) {
    
    validate<baseline_greschd::sim, baseline_greschd::sim, addon::std_mt_rng>();
    validate<v1_int2t::sim, v1_int2t::sim, addon::std_mt_rng>();
    validate<v1_int2t::sim, baseline_greschd::sim, addon::std_mt_rng>();
    
    validate<baseline_mskoenz::sim, baseline_mskoenz::sim, addon::lag_fib_rng>();
    validate<baseline_mskoenz::sim, baseline_mskoenz::sim, addon::std_mt_rng>();
    
    validate<baseline_mskoenz::sim, baseline_greschd::sim, addon::lag_fib_rng>();
    validate<baseline_mskoenz::sim, baseline_greschd::sim, addon::std_mt_rng>();
    
    return 0;
}
