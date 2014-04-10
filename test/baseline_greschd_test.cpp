// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    25.03.2014 00:42:33 CET
// File:    test.cpp

#include <sim/baseline_greschd_sim.hpp>
#include <grid/baseline_greschd_grid.hpp>
#include <matrix/baseline_greschd_matrix.hpp>
#include <rng/std_mt_rng.hpp>

#include <iostream>

int main(int argc, char* argv[]) {
    
    using namespace mc_potts;
    
    constexpr int size = 30;
    
    baseline_greschd_sim::impl<size, size, size, addon::std_mt_rng, baseline_greschd_grid, baseline_greschd_matrix> testsim(1e-8, 200);
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
