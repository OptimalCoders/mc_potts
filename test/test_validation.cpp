// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    26.03.2014 10:11:58 CET
// File:    test_validation.cpp

#include <validation.hpp>
#include <std_mt_rng.hpp>
#include <baseline_impl_greschd/sim.hpp>

#include <iostream>

using namespace mc_potts;


int main(int argc, char* argv[]) {
    
    typedef vector<int_<1>, int_<2>, int_<4>>::type vec_type;

    for_each<vec_type>(compare<sim_baseline_greschd, sim_baseline_greschd, 4, std_mt_rng>());
    
    return 0;
}
