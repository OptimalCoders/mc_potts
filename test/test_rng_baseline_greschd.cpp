// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    25.03.2014 10:07:31 CET
// File:    test_rng_baseline_greschd.cpp

#include <iostream>
#include <rng/std_mt_rng.hpp>

using namespace addon;

template<class T>
using rng_type = std_mt_rng<T>;
    
int main(int argc, char* argv[]) {
    
    rng_type<uint8_t> rng;
    rng.set_seed(42);
    for(uint i = 0; i < 10; ++i) {
        std::cout << int(rng()) << " ";
    }
    std::cout << std::endl;
    rng_type<int> rng2;
    rng2.set_seed(42);
    for(uint i = 0; i < 10; ++i) {
        std::cout << int(rng2()) << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
