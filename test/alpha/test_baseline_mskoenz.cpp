// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    25.03.2014 15:04:30 CET
// File:    test_baseline_mskoenz.cpp

#include <addon/ppm_picture.hpp>
#include <alpha.hpp>

#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    
    const int N = 40;
    
	addon::detail::lag_fib_engine.seed(0);
    mc_potts::baseline_mskoenz::sim::impl<N, N, N, addon::lag_fib_rng> s(2, 100);
    std::vector<std::vector<int8_t>> temp(N, std::vector<int8_t>(N));
    s.thermalize();
    //------------------- ppm pic -------------------
    addon::ppm_picture_class ppm("out");
    ppm.set_cellsize(5); //each point is 5x5 pixel
    ppm.set_color(mc_potts::baseline_mskoenz::n_state);
        
    //------------------- thermalize -------------------
    //~ for(uint i = 0; i < 300; ++i) {
        //~ s.set_T(1000*(1 - double(i)/300)); //cooling down
		//~ s.thermalize();
        //~ s.update();
        //~ s.measure();
        
        //~ for(uint i = 0; i < N; ++i) {
			//~ for(uint j = 0; j < N; ++j) {
				//~ temp[i][j] = s.get(i, j, 0);
			//~ }
		//~ }
        //~ ppm.print(temp, i); //ppm picture
    //~ }
    s.print();
    
    return 0;
}
