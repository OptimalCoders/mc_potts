// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    22.03.2014 18:21:30 CET
// File:    test.cpp

#include <iostream>
#include "../src/lib/mc.hpp"

using namespace mc_potts;

int main(int argc, char* argv[]) {
    mc_cl<100, 100, 4> a(1e-8);
    a. update(200000);
    a.print_observables();
    a.picture("before");
    a.set_T(100);
    a. update(200000);
    a.print_observables();
    a.picture("after");
    return 0;
}
