// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    26.03.2014 10:11:58 CET
// File:    test_validation.cpp

#include <rng/std_mt_rng.hpp>
#include <rng/lag_fib_rng.hpp>
#include <alpha/baseline_impl_greschd/sim.hpp>
#include <alpha/v1_int2t/sim.hpp>
#include <alpha/baseline_impl_mskoenz/sim.hpp>

#include <sim/msk_v1_sim.hpp>
#include <sim/baseline_greschd_sim.hpp>

#include <grid/msk_v1_pbc.hpp>
#include <grid/baseline_greschd_grid.hpp>

#include <matrix/msk_v1_zorder.hpp>
#include <matrix/int2t_v01_matrix.hpp>
#include <matrix/baseline_greschd_matrix.hpp>

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
