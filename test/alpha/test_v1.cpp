// Author:  Dominik Gresch <greschd@ethz.ch>
// Date:    26.03.2014 23:18:26 CET
// File:    test_v1.cpp

#include <iostream>
#include <alpha.hpp>

int main(int argc, char* argv[]) {
    
    using namespace mc_potts;
    
    constexpr int size = 30;
    
    v1_int2t::sim::impl<size, size, size, addon::std_mt_rng> testsim(1e-8, 200);
    testsim.thermalize();
    for(uint i = 0; i < 10; ++i) {
        testsim.update();
        testsim.measure();
    }
    
    result_struct energy = testsim.energy();
    std::cout << energy.n << std::endl;
    std::cout << energy.mean << std::endl;
    std::cout << energy.dev << std::endl;
    std::cout << energy.err << std::endl;
    
    //~ testsim.picture_slice(int(size / 2), "0");
    //~ for(uint i = 0; i < 25; ++i) {
        //~ for(uint i = 0; i < 10; ++i) {
            //~ testsim.update();
        //~ }
        //~ testsim.picture_slice(int(size / 2), std::to_string(i + 1));
    //~ }
    
    
    
    return 0;
}
