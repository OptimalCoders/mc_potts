// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    26.03.2014 21:15:05 CET
// File:    validate_interface.hpp

#ifndef __VALIDATE_INTERFACE_HEADER
#define __VALIDATE_INTERFACE_HEADER

#include <interface/rng_interface.hpp>
#include <interface/sim_interface.hpp>
#include <interface/grid_interface.hpp>
#include <interface/matrix_interface.hpp>

#define VALIDATE_RNG(RNG)                                   \
{                                                           \
    /*------------- valid expressions for rng -----------*/ \
    RNG<double> rng(0, 1);                                  \
    RNG<double>::seed();                                    \
    RNG<double>::seed(8);                                   \
    RNG<int> rng2;                                          \
    rng2.seed(8);                                           \
    rng2.seed();                                            \
    rng();                                                  \
    RNG<double>::name();                                    \
}                                                           //

#define VALIDATE_MATRIX(MAT)                                \
{                                                           \
    typename MAT::template impl<16, 16, 16> mat;            \
    mat.get(0, 0, 0);                                       \
    mat.set(0, 0, 0, 0);                                    \
    MAT::name();                                            \
}                                                           //

#define VALIDATE_GRID(GRI)                                  \
{                                                           \
    typename GRI::template impl<16, 16, 16, interface::matrix_proto_type> grid; \
    grid.get_nn(0, 0, 0);                                   \
    grid.get(0, 0, 0);                                      \
    grid.set(0, 0, 0, 0);                                   \
    GRI::name();                                            \
}                                                           //

#define VALIDATE_SIM(SIM)                                   \
{                                                           \
    /*------------- valid expressions for sim -----------*/ \
    typename SIM::template impl<16, 16, 16, interface::rng_proto_type, interface::grid_proto_type, interface::matrix_proto_type> sim(10, 10, 1); \
    sim.thermalize();                                       \
    sim.update();                                           \
    sim.measure();                                          \
    sim.energy();                                           \
    sim.magn();                                             \
    sim.set_T(0.1);                                         \
    sim.clear();                                            \
    sim.get(0, 0, 0);                                       \
    SIM::name();                                            \
    sim.spec();                                             \
}                                                           //

#endif //__VALIDATE_INTERFACE_HEADER
