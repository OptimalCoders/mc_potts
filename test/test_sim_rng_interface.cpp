// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    24.03.2014 21:21:24 CET
// File:    test_prototype_interface.cpp

#include <interface/validate_interface.hpp>
//------------------- prototype -------------------
#include <interface/rng_interface.hpp>
#include <interface/sim_interface.hpp>
//------------------- baseline_greschd -------------------
#include <baseline_impl_greschd/sim_baseline_greschd.hpp>
#include <rng/std_mt_rng.hpp>
//------------------- baseline_mskoenz -------------------
#include <baseline_impl_mskoenz/sim.hpp>
#include <rng/lag_fib_rng.hpp>

int main(int argc, char* argv[]) {
    
    VALIDATE_INTERFACE(interface::sim_proto_type, interface::rng_proto_type)
    VALIDATE_INTERFACE(mc_potts::sim_baseline_greschd, addon::std_mt_rng)
    VALIDATE_INTERFACE(mc_potts::baseline_mskoenz_struct, addon::lag_fib_rng)
    
    return 0;
}
