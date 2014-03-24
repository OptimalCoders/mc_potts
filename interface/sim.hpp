// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    24.03.2014 15:19:21 CET
// File:    sim.hpp

#ifndef __SIM_HEADER
#define __SIM_HEADER

struct result_struct {
    double mean;
    double dev;
    double err;
    uint64_t n;
};

template< int L1
        , int L2
        , int L3
        , int S
        , class template RNG<typename> >
class sim_proto_type {
    sim_proto_type(uint32_t const & N_therm, uint32_t const & N_update = L1*L2*L3, double const & T_init) {
    }
    void thermalize() {
    }
    void update() { //ensure decorr
    }
    void measure() { //feedeing
    }
    result_struct energy() const {
    }
    result_struct magn() const {
    }
    void set_T() {
    }
    void clear() {
    }
    uint8_t get(  uint32_t const & l1
                , uint32_t const & l2
                , uint32_t const & l3) const {
    }
};

/*
init()

set random state with dedicated (0, S) rng local.

update()

L1 * L2 * L3 attempted single spin updates

set_T()

clear "accumulator"
precompute prob

*/

#endif //__SIM_HEADER
