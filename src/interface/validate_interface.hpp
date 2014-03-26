// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    26.03.2014 21:15:05 CET
// File:    validate_interface.hpp

#ifndef __VALIDATE_INTERFACE_HEADER
#define __VALIDATE_INTERFACE_HEADER

#define VALIDATE_INTERFACE(SIM, RNG)                    \
{                                                       \
    /*------------- valid expressions for rng -----------*/ \
    RNG<double> rng(0, 1);                                  \
    RNG<int> rng2;                                          \
    rng.set_seed(8);                                        \
    rng2.seed();                                            \
    rng();                                                  \
    /*------------- valid expressions for sim -----------*/ \
    typename SIM::template impl<10, 10, 10, 4, RNG> sim(10, 10, 1);    \
    sim.thermalize();                                       \
    sim.update();                                           \
    sim.measure();                                          \
    sim.energy();                                           \
    sim.magn();                                             \
    sim.set_T(0.1);                                         \
    sim.clear();                                            \
    sim.get(0, 0, 0);                                       \
}


#endif //__VALIDATE_INTERFACE_HEADER
