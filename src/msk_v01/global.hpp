// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    21.03.2014 09:57:00 CET
// File:    global.hpp

#ifndef ___MSK_V01_GLOBAL_HEADER
#define ___MSK_V01_GLOBAL_HEADER

#include <addon/debug.hpp>

//  +---------------------------------------------------+
//  |                   typedefs                        |
//  +---------------------------------------------------+
namespace mc_potts {
    namespace msk_v1 {
        //------------------- state type -------------------
        using state_type = int8_t; //must be signed
        const state_type max_state = 4;
        const uint n_neighbour = 6;
        const uint n_state = max_state;
        
        //------------------- technical typedefs -------------------
        using index_type = int;
    }//end namespace msk_v1
}//end mc_namespace 

#endif //___MSK_V01_GLOBAL_HEADER
