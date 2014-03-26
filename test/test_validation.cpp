// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    26.03.2014 10:11:58 CET
// File:    test_validation.cpp

#include <rng/std_mt_rng.hpp>
#include <baseline_impl_greschd/sim_baseline_greschd.hpp>
#include <baseline_impl_mskoenz/sim.hpp>
#include <validation.hpp>

#include <iostream>

using namespace mc_potts;


int main(int argc, char* argv[]) {
    
    validate<sim_baseline_greschd, sim_baseline_greschd, 4, addon::std_mt_rng>();
    validate<baseline_mskoenz_struct, baseline_mskoenz_struct, 4, addon::std_mt_rng>();
    validate<baseline_mskoenz_struct, sim_baseline_greschd, 4, addon::std_mt_rng>();
    
    return 0;
}
