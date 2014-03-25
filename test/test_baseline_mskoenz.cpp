// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    25.03.2014 15:04:30 CET
// File:    test_baseline_mskoenz.cpp

#include <addon/ppm_picture.hpp>
#include <baseline_impl_mskoenz/sim.hpp>

#include <iostream>

int main(int argc, char* argv[]) {
    
    //------------------- test grid -------------------
    potts::grid_class<5, 10> g;
    
    g.ref(1, 0) = 1;
    g.ref(0, 1) = 1;
    g.ref(4, 0) = 1;
    g.ref(0, 9) = 1;
    g.print();
    
    DEBUG_VAR((int)g.neighbour_diff(0, 0));
    
    //------------------- set seed -------------------
    addon::global_seed.set(43);
    
    //------------------- test sim -------------------
    potts::sim_class<50, 50> s;
    s.set_T(2);
    
    //------------------- ppm pic -------------------
    addon::ppm_picture_class ppm("out");
    ppm.set_cellsize(5); //each point is 5x5 pixel
    ppm.set_color(potts::n_state);
        
    //------------------- thermalize -------------------
    for(uint i = 0; i < 1000; ++i) {
        s.update();
    }
    for(uint i = 0; i < 300; ++i) {
        s.set_T(2*(1 - double(i)/300)); //cooling down
        s.update();
        s.measure();
        //~ ppm.print(s.grid(), i); //ppm picture
    }
    s.print();
    
    return 0;
}
