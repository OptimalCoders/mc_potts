// Author:  Dominik Gresch <greschd@ethz.ch>
// Date:    27.03.2014 00:34:57 CET
// File:    test_int2t_vec.cpp

#include <iostream>
#include <v1_int2t/int2t_vec.hpp>

int main(int argc, char* argv[]) {
    
    
    constexpr uint L1 = 2;
    constexpr uint L2 = 3;
    constexpr uint L3 = 4;
    int2t_3Dvec<L1, L2, L3> a;
    
    for(uint i = 0; i < L1; ++i) {
        for(uint j = 0; j < L2; ++j) {
            for(uint k = 0; k < L3; ++k) {
                a.set(i, j, k, (i*j*k)% 4);
            }
        }
    }
    
    for(uint i = 0; i < L1; ++i) {
        for(uint j = 0; j < L2; ++j) {
            for(uint k = 0; k < L3; ++k) {
                //~ if(a.get(i, j, k) != (i * j * k) % 4) {
                    //~ std::cout << int(a.get(i, j, k)) << " " << (i * j * k) % 4 << std::endl;
                //~ }
                assert(a.get(i, j, k) == (i * j * k) % 4);
            }
        }
    }
    
    return 0;
}
