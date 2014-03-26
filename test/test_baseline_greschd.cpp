// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    25.03.2014 00:42:33 CET
// File:    test.cpp

#include <iostream>
#include <baseline_impl_greschd/sim_baseline_greschd.hpp>
#include <rng/std_mt_rng.hpp>

int main(int argc, char* argv[]) {
    
    using namespace mc_potts;
    
    constexpr int size = 30;
    
    sim_baseline_greschd::impl<size, size, size, 4, addon::std_mt_rng> testsim(1e-8, 200);
    testsim.thermalize();
    //~ for(uint i = 0; i < 10; ++i) {
        //~ testsim.update();
        //~ testsim.measure();
    //~ }
    
    result_struct energy = testsim.energy();
    std::cout << energy.n << std::endl;
    std::cout << energy.mean << std::endl;
    std::cout << energy.dev << std::endl;
    std::cout << energy.err << std::endl;
    
    testsim.picture_slice(int(size / 2), "0");
    for(uint i = 0; i < 25; ++i) {
        for(uint i = 0; i < 10; ++i) {
            testsim.update();
        }
        testsim.picture_slice(int(size / 2), std::to_string(i + 1));
    }
    
    
    
    return 0;
}
