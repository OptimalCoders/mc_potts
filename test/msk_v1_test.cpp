// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    09.04.2014 17:10:29 CEST
// File:    msk_v1_test.cpp

#include <sim/msk_v1_sim.hpp>
#include <grid/msk_v1_pbc.hpp>
#include <rng/lag_fib_rng.hpp>
#include <matrix/msk_v1_zorder.hpp>

#include <baseline_impl_mskoenz/sim.hpp>
#include <iostream>

#include <validation.hpp>

int main(int argc, char* argv[]) {
    
    addon::lag_fib_rng<double> r;
    
    std::cout << r() << std::endl;
    
    int const L = 32;
    
    mc_potts::msk_v1_sim::impl<L, L, L, addon::lag_fib_rng
                                      , mc_potts::msk_v1_pbc
                                      , mc_potts::msk_v1_zorder
                                      > s(1, 10, 2);
    
    s.update();
    
    
    return 0;
}
