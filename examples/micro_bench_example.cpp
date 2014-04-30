// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    30.04.2014 09:19:56 CEST
// File:    micro_bench_example.cpp

#include <iostream>
#include <addon/micro_bench.hpp>

double load(int N) {
    double res = 0;
    for(uint i = 0; i < 100; ++i) {
        for(uint j = 0; j < 100; ++j) {
            res += i*j*N;
        }
    }
    return res;
}

double fct(int N) {
    START_MICRO("fct")
    double res = 0;
    
    START_MICRO("p1")
    res += load(N);
    
    //NEXT_MICRO(name) === STOP_MICRO(); START_MICRO(name)
    NEXT_MICRO("p2")
    res += load(N);
    STOP_MICRO()
    
    STOP_MICRO()
    return res;
}

int main(int argc, char* argv[]) {
    double res = 0;
    for(uint i = 0; i < 100; ++i) {
        START_MICRO("main")
        res += fct(i);
        STOP_MICRO()
    }
    std::cout << res << std::endl;
    P_MICRO()
    return 0;
}
