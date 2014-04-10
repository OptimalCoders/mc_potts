// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    24.03.2014 21:21:24 CET
// File:    test_prototype_interface.cpp

#include <beta.hpp>
#include <alpha.hpp>

#include <interface/validate_interface.hpp>

int main(int argc, char* argv[]) {
    VALIDATE_RNG(interface::rng_proto_type)
    VALIDATE_MATRIX(interface::matrix_proto_type)
    VALIDATE_GRID(interface::grid_proto_type)
    VALIDATE_SIM(interface::sim_proto_type)
    
    VALIDATE_RNG(addon::lag_fib_rng)
    VALIDATE_RNG(addon::std_mt_rng)
    
    VALIDATE_SIM(mc_potts::msk_v1::sim)
    VALIDATE_SIM(mc_potts::dag_v02::sim)
    VALIDATE_SIM(mc_potts::v1_int2t::sim)
    VALIDATE_SIM(mc_potts::baseline_mskoenz::sim)
    VALIDATE_SIM(mc_potts::baseline_greschd::sim)
    
    VALIDATE_MATRIX(mc_potts::msk_v1_zorder)
    VALIDATE_GRID(mc_potts::msk_v1_pbc)
    VALIDATE_SIM(mc_potts::msk_v1_sim)
    
    return 0;
}
