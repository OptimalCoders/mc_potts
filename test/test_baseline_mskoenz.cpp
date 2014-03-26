// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    25.03.2014 15:04:30 CET
// File:    test_baseline_mskoenz.cpp

#include <addon/ppm_picture.hpp>
#include <baseline_impl_mskoenz/sim.hpp>
#include <std_mt_rng.hpp>

#include <iostream>

template<typename T>
struct dummy_rng_struct {
};


int main(int argc, char* argv[]) {
    //------------------- set seed -------------------
    addon::global_seed.set(43);
    
    //------------------- test sim -------------------
    // mc_potts::std_mt_rng
    typename mc_potts::baseline_mskoenz_struct::template impl<50, 50, 50, 4, addon::msk_rng> s(0.1, 10, 50*50);
    s.set_T(2);
    
    //------------------- ppm pic -------------------
    addon::ppm_picture_class ppm("out");
    ppm.set_cellsize(5); //each point is 5x5 pixel
    ppm.set_color(mc_potts::n_state);
        
    //------------------- thermalize -------------------
    for(uint i = 0; i < 1000; ++i) {
        s.update();
    }
    for(uint i = 0; i < 300; ++i) {
        s.set_T(2*(1 - double(i)/300)); //cooling down
        s.update();
        s.measure();
        //~ ppm.print(s.grid()[0], i); //ppm picture
    }
    s.print();
    
    return 0;
}
