// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    24.03.2014 15:03:04 CET
// File:    rng.hpp

#ifndef __RNG_HEADER
#define __RNG_HEADER

/*
rng_proto_type<double> rng() // [0, 1)
rng_proto_type<double> rng(5, 8) // [5, 9)

rng_proto_type<int> rng() // {0, 1}
rng_proto_type<int> rng(0, size) // {0, ... , size - 1}

rng.set_seed(double);
rng.seed();

rng();
*/

namespace interface {
    template<typename T>
    class rng_proto_type {
    public:
        rng_proto_type(T const & lower = T(), T const & upper = T()) {
        }
        T operator()() {
            return T();
        }
        void set_seed(uint32_t const & seed) {
        }
        uint32_t const & seed() const {
            return seed_;
        }
    private:
        uint32_t seed_;
    };
}//end namespace interface

#endif //__RNG_HEADER
