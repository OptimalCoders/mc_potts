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

    using namespace boost::mpl;

    template<class IMPL1, class IMPL2, int S, template<class> class RNG>
    struct compare {
        template< typename U > void operator()(U) {
            
            double T1 = 10;
            double T2 = 0.5;
            
            uint32_t N_therm = 200;
            uint32_t updates = 50;
            
            RNG<int> rng_handle;
            int seed(std::time(NULL));
            
            
            //impl1
            rng_handle.set_seed(seed);
            typename IMPL1::template impl<U::value, U::value, U::value, S, RNG > impl1(T1, N_therm);
            impl1.thermalize();
            for(uint i = 0; i < updates; ++i) {
                impl1.update();
            }
            impl1.set_T(T2);
            for(uint i = 0; i < updates; ++i) {
                impl1.update();
            }
            
            // impl2
            rng_handle.set_seed(seed);
            typename IMPL2::template impl<U::value, U::value, U::value, S, RNG > impl2(T1, N_therm);
            impl2.thermalize();
            for(uint i = 0; i < updates; ++i) {
                impl2.update();
            }
            impl2.set_T(T2);
            for(uint i = 0; i < updates; ++i) {
                impl2.update();
            }
            
            for(uint i = 0; i < U::value; ++i) {
                for(uint j = 0; j < U::value; ++j) {
                    for(uint k = 0; k < U::value; ++k) {
                        assert(impl1.get(i, j, k) == impl2.get(i, j, k));
                    }
                }
            }
        }
    };

} // namespace mc_potts

#endif //__VALIDATION_HEADER
