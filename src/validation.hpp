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

    template<   class IMPL1, 
                class IMPL2, 
                template<typename> class RNG,
                class GRID1 = void, 
                class GRID2 = void, 
                class MATRIX1 = void, 
                class MATRIX2 = void >
    struct compare {
        template< typename U > void operator()(U) {
            
            double T1 = 10;
            double T2 = 0.5;
            
            uint32_t N_therm = 200;
            uint32_t updates = 10;
            
            int seed(std::time(NULL));
            
            // testing sequence
            #define CALL(x, y, grid, mat)                                                                   \
            RNG<int>::seed(seed);                                                                           \
            typename y::template impl< U::value, U::value, U::value, RNG, grid, mat > x(T1, N_therm, 10);   \
            x.thermalize();                                                                                 \
            for(uint i = 0; i < updates; ++i) {                                                             \
                x.update();                                                                                 \
            }                                                                                               \
            x.set_T(T2);                                                                                    \
            for(uint i = 0; i < updates; ++i) {                                                             \
                x.update();                                                                                 \
            }                                                                                               //
            
            CALL(impl1, IMPL1, GRID1, MATRIX1)
            CALL(impl2, IMPL2, GRID2, MATRIX2)
            
            // compare elements
            for(uint i = 0; i < U::value; ++i) {
                for(uint j = 0; j < U::value; ++j) {
                    for(uint k = 0; k < U::value; ++k) {
                        assert(impl1.get(i, j, k) == impl2.get(i, j, k));
                        //~ if(impl1.get(i, j, k) != impl2.get(i, j, k)) { //DEBUG
                            //~ std::cout << i << " " << j << " " << k << std::endl;
                            //~ std::cout << int(impl1.get(i, j, k)) << " " << int(impl2.get(i, j, k)) << "\n" << std::endl;
                        //~ }
                    }
                }
            }
        }
    };
    
    template<   class IMPL1, 
                class IMPL2, 
                template<typename> class RNG, 
                class GRID1 = void,
                class GRID2 = void,
                class MATRIX1 = void,
                class MATRIX2 = void>
    void validate() {
        typedef boost::mpl::vector<boost::mpl::int_<1>, boost::mpl::int_<2>, boost::mpl::int_<4>, boost::mpl::int_<8>, boost::mpl::int_<256>>::type vec_type;
        boost::mpl::for_each<vec_type>(compare<IMPL1, IMPL2, RNG, GRID1, GRID2, MATRIX1, MATRIX2>());
    }

} // namespace mc_potts

#endif //__VALIDATION_HEADER
