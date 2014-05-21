// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    25.03.2014 00:42:33 CET
// File:    test.cpp

#include <beta.hpp>

#include <iostream>

int main(int argc, char* argv[]) {
    
    using namespace mc_potts;
    
    constexpr int size = 200;
    
    baseline_greschd_sim::impl<size, size, size, addon::std_mt_rng, baseline_greschd_grid, int2t_v01_matrix> testsim(1e-8, 200);
    testsim.thermalize();
    for(uint i = 0; i < 100; ++i) {
        testsim.update();
        testsim.measure();
    }
    
    //~ result_struct energy = testsim.energy();
    //~ std::cout << energy.n << std::endl;
    //~ std::cout << energy.mean << std::endl;
    //~ std::cout << energy.dev << std::endl;
    //~ std::cout << energy.err << std::endl;
    
    return 0;
}
