// Author:  Dominik Gresch <greschd@ethz.ch>
// Date:    20.05.2014 23:23:38 CEST
// File:    mc_potts.hpp

#ifndef __MC_POTTS_HEADER
#define __MC_POTTS_HEADER

#include <beta.hpp>
#include <../install/install.hpp>

namespace mc_potts {
    namespace install {
        //----------------------------------------------------------------------//
        //                      finding the nearest size                        //
        //----------------------------------------------------------------------//

        template<int S, int N, bool B>
        struct size_helper {
            static const int size = size_helper<S, N - 1, S >= (sizes[N] + sizes[N - 1])/ 2 >::size;
        };

        template<int S, int N>
        struct size_helper<S, N, true> {
            static const int size = sizes[N + 1];
        };

        template<int S>
        struct size_helper<S, 0, false> {
            static const int size = sizes[0];
        };

        template<int S>
        struct size_helper<S, num_sizes - 1, true> {
            static const int size = sizes[num_sizes - 1];
        };

        template<int S>
        struct size_finder {
            static const int size = size_helper<S, num_sizes - 1, S >= sizes[num_sizes - 1] >::size; 
        };
        
    }// detail
    
template<int N>
using potts_model = typename install::opt<install::size_finder<N>::size>::template impl<N>;
    
}// mc_potts

#endif //__MC_POTTS_HEADER
