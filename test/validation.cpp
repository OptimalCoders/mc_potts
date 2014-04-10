// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    26.03.2014 10:11:58 CET
// File:    test_validation.cpp

#include <beta.hpp>
#include <alpha.hpp>
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
    validate<baseline_mskoenz::sim, msk_v1_sim, addon::lag_fib_rng, void, msk_v1_pbc, void, msk_v1_zorder>();

    validate<baseline_greschd::sim, baseline_greschd_sim, addon::std_mt_rng, void, baseline_greschd_grid, void, baseline_greschd_matrix>();
    validate<baseline_greschd::sim, baseline_greschd_sim, addon::std_mt_rng, void, baseline_greschd_grid, void, int2t_v01_matrix>();
    
    return 0;
}
