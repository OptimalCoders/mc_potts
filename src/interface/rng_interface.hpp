// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    24.03.2014 15:03:04 CET
// File:    rng.hpp

#ifndef __RNG_INTERFACE_HEADER
#define __RNG_INTERFACE_HEADER

#include <types.hpp>
#include <string>
/*
rng_proto_type<double> rng; // [0, 1)
rng_proto_type<double> rng(5, 8); // [5, 9)

rng_proto_type<int> rng; // {0, 1}
rng_proto_type<int> rng(0, size); // {0, ... , size - 1}

rng.seed(double);
rng.seed();

rng();
*/

namespace interface {
    template<typename T>
    class rng_proto_type {
        using seed_type = mc_potts::seed_type;
    public:
        rng_proto_type(T const & lower = T(), T const & upper = T()) {
        }
        T operator()() {
            return T();
        }
        static void seed(seed_type const & seed) {
        }
        static seed_type const & seed() {
            return seed_;
        }
        static std::string name() {
            return "rng_proto_type";
        }
    private:
        static uint32_t seed_;
    };
}//end namespace interface

#endif //__RNG_INTERFACE_HEADER
