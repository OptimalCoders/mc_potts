// Author:  Dominik Gresch <greschd@ethz.ch>
// Date:    01.05.2014 04:38:51 CEST
// File:    test_mkl_mt_rng.cpp

#include <beta.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    
    addon::mkl_mt_rng<double> rng(-1., 1.);
    for(uint i = 0; i < 100; ++i) {
        std::cout << rng() << std::endl;
    }
    
    return 0;
}
