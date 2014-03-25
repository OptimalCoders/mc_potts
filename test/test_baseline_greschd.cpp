// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    25.03.2014 00:42:33 CET
// File:    test.cpp

#include <iostream>
#include <baseline_impl_greschd/sim.hpp>
#include <baseline_impl_greschd/rng.hpp>

int main(int argc, char* argv[]) {
    
    using namespace mc_potts;
    
    sim_baseline_greschd::impl<100, 100, 100, 4, rng_baseline_greschd> testsim(1, 20000);
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
    
    
    return 0;
}
