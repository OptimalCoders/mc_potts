// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    21.03.2014 09:53:10 CET
// File:    main.cpp

#include <iostream>
#include "sim.hpp"

int main(int argc, char* argv[]) {
    
    //------------------- test grid -------------------
    potts::grid_class<5, 10> g;
    
    g.ref(1, 0) = 1;
    g.ref(0, 1) = 1;
    g.ref(4, 0) = 1;
    g.ref(0, 9) = 1;
    g.print();
    
    DEBUG_VAR((int)g.neighbour_diff(0, 0));
    
    //------------------- test sim -------------------
    potts::sim_class<5, 10> s;
    
    
    for(uint i = 0; i < 1000; ++i) {
        s.update();
        s.measure();
    }
    
    s.print();
    
    return 0;
}
