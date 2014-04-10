// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    25.03.2014 15:04:30 CET
// File:    test_baseline_mskoenz.cpp

#include <addon/ppm_picture.hpp>
#include <alpha.hpp>

#include <iostream>

int main(int argc, char* argv[]) {
    DEBUG_MSG("main start")
    //------------------- test sim -------------------
    int const L = 204;
    addon::std_mt_rng<int>::seed(0);
    mc_potts::baseline_mskoenz::sim::impl<L, L, L, addon::std_mt_rng> s(0.1, 10, 2);
    //~ s.thermalize();
    //~ s.update();
    //~ s.update();
    //~ s.update();
    DEBUG_MSG("greschd")
    addon::std_mt_rng<int>::seed(0);
    mc_potts::baseline_greschd::sim::impl<L, L, L, addon::std_mt_rng> s2(0.1, 10, 2);
    //~ s2.thermalize();
    //~ s2.update();
    //~ s2.update();
    //~ s2.update();
    
    for(uint i = 0; i < 2; ++i) {
        for(uint j = 0; j < 5; ++j) {
            for(uint k = 0; k < 5; ++k) {
                //~ std::cout << (s2.get(i, j, k) == s.get(i, j, k)) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    /*
    addon::detail::lag_fib_engine.seed(0);
    mc_potts::baseline_mskoenz::sim::impl<2, 5, 5, 4, addon::lag_fib_rng> s(0.1, 10, 2);
    s.thermalize();
    //------------------- ppm pic -------------------
    addon::ppm_picture_class ppm("out");
    ppm.set_cellsize(5); //each point is 5x5 pixel
    ppm.set_color(mc_potts::baseline_mskoenz::n_state);
        
    //------------------- thermalize -------------------
    for(uint i = 0; i < 1000; ++i) {
        s.update();
    }
    for(uint i = 0; i < 300; ++i) {
        s.set_T(2*(1 - double(i)/300)); //cooling down
        s.update();
        s.measure();
        //~ ppm.print(s.grid()[0], i); //ppm picture
    }
    s.print();
    */
    return 0;
}
