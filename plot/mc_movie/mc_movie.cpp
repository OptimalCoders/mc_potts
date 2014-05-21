// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    21.05.2014 11:41:36 CEST
// File:    mc_movie.cpp

#include <addon/ppm_picture.hpp>
#include <alpha.hpp>
#include <beta.hpp>

#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    
    const int N = 100;
    
    addon::detail::lag_fib_engine.seed(0);
    mc_potts::baseline_mskoenz::sim::impl<N, N, N, addon::custom_mt_rng> s(2, 30);
    
    std::vector<std::vector<int8_t>> temp(N, std::vector<int8_t>(N));
    s.thermalize();
    
    //------------------- ppm pic -------------------
    std::string path = argv[0];
    path.erase(path.end() - 8, path.end());
    path += "out";
    //~ /*
    addon::ppm_picture_class ppm(path);
    ppm.set_cellsize(5); //each point is 5x5 pixel
    ppm.set_color(mc_potts::baseline_mskoenz::n_state);
        
    //------------------- thermalize -------------------
    for(uint i = 0; i < 300; ++i) {
        s.set_T(6 - 4*double(i)/300); //cooling down
		s.thermalize();
        //~ s.update();
        //~ s.measure();
        
        for(uint i = 0; i < N; ++i) {
			for(uint j = 0; j < N; ++j) {
                temp[i][j] = s.get(i, j, 0);
            }
        }
        ppm.print(temp, i); //ppm picture
    }
    //~ s.print();
    
    for(uint i = 0; i < 100; ++i) {
        s.thermalize();
        s.thermalize();
        s.thermalize();
        
        for(uint i = 0; i < N; ++i) {
            for(uint j = 0; j < N; ++j) {
                temp[i][j] = s.get(i, j, 0);
            }
        }
        ppm.print(temp, 300 + i); //ppm picture
    }
    
    /**/
    return 0;
}
