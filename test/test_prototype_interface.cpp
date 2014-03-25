// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    24.03.2014 21:21:24 CET
// File:    test_prototype_interface.cpp

#include <iostream>
#include <interface/rng_interface.hpp>
#include <interface/sim_interface.hpp> 

template<typename T>
using rng_type = interface::rng_proto_type<T>;

using sim_type = interface::sim_proto_type;

int main(int argc, char* argv[]) {
    
    //------------------- valid expressions for rng -------------------
    rng_type<double> rng(0, 1);
    rng_type<int> rng2;
    
    rng.set_seed(8);
    rng2.seed();
    
    rng();
    
    //------------------- valid expressions for sim -------------------
    typename sim_type::template impl<10, 10, 10, 4, rng_type> sim(10000, 1000, 0.1);
    
    
    return 0;
}
