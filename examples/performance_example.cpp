// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    26.03.2014 15:45:36 CET
// File:    performance_example.cpp

#include <iostream>
#include <addon/performance.hpp>

int main(int argc, char* argv[]) {
    
    //the header already has a performance object called addon::clock
    
    //define some computation to burn time
    double res;
    
    #define LOAD()                          \
    res = 0;                                \
    for(uint i = 0; i < 10000; ++i) {       \
        for(uint j = 0; j < 10000; ++j) {   \
            res += i * j;                   \
        }                                   \
    }                                       \
    std::cout << res << std::endl;
    
    //cout << res is needed, otherwise -O3 removes LOAD
    
    // start clock
    addon::clock.start();
    LOAD();
    //stop clock
    addon::clock.stop();
    
    //clock can be printed
    std::cout << addon::clock << std::endl;
    //addon::clock.print() is als valid
    
    //available methods
    std::cout << addon::clock.cycles() << std::endl;
    std::cout << addon::clock.sec() << std::endl;
    std::cout << addon::clock.msec() << std::endl;
    std::cout << addon::clock.usec() << std::endl;
    
    //clear
    addon::clock.clear();
    
    return 0;
}
