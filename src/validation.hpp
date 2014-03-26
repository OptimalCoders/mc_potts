// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    26.03.2014 10:09:33 CET
// File:    validation.hpp

#ifndef __VALIDATION_HEADER
#define __VALIDATION_HEADER


#include <boost/mpl/vector.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/int.hpp>

#include <iostream>
#include <vector>
#include <ctime>
#include <cassert>

namespace mc_potts {

    template<class IMPL1, class IMPL2, int S, template<class> class RNG>
    struct compare {
        template< typename U > void operator()(U) {
            
            double T1 = 10;
            double T2 = 0.5;
            
            uint32_t N_therm = 200;
            uint32_t updates = 50;
            
            RNG<int> rng_handle;
            int seed(std::time(NULL));
            
            // testing sequence
            #define CALL(x, y) \
            rng_handle.set_seed(seed); \
            typename y::template impl<U::value, U::value, U::value, S, RNG > x(T1, N_therm); \
            x.thermalize(); \
            for(uint i = 0; i < updates; ++i) { \
                x.update(); \
            } \
            x.set_T(T2); \
            for(uint i = 0; i < updates; ++i) { \
                x.update(); \
            } \
            
            CALL(impl1, IMPL1)
            CALL(impl2, IMPL2)
            
            // compare elements
            for(uint i = 0; i < U::value; ++i) {
                for(uint j = 0; j < U::value; ++j) {
                    for(uint k = 0; k < U::value; ++k) {
                        assert(impl1.get(i, j, k) == impl2.get(i, j, k));
                    }
                }
            }
        }
    };
    
    template<class IMPL1, class IMPL2, int S, template<class> class RNG>
    void validate() {
        typedef boost::mpl::vector<boost::mpl::int_<1>, boost::mpl::int_<2>, boost::mpl::int_<4>>::type vec_type;
        boost::mpl::for_each<vec_type>(compare<IMPL1, IMPL2, S, RNG>());
    }

} // namespace mc_potts

#endif //__VALIDATION_HEADER
