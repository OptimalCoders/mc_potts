// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    29.04.2014 21:47:03 CEST
// File:    test_micro_bench.cpp

#include <iostream>
#include <addon/micro_bench.hpp>
#include <beta.hpp>

int main(int argc, char* argv[]) {
    using namespace mc_potts;
    const int L = 30;
    const int up = 10;
    
    msk_v1_sim::impl<L, L, L, addon::std_mt_rng
                            , msk_v1_pbc
                            , msk_v2_dynamic_zip
                            > s7(10, 10, up);
    
    for(uint i = 0; i < 1000; ++i) {
        START_MICRO("main")
        s7.update();
        STOP_MICRO()
    }
    P_MICRO()
    return 0;
}
