// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    21.03.2014 09:57:00 CET
// File:    global.hpp

#ifndef __GLOBAL_HEADER
#define __GLOBAL_HEADER

#include <addon/debug.hpp>

//  +---------------------------------------------------+
//  |                   typedefs                        |
//  +---------------------------------------------------+
namespace mc_potts {
    //------------------- state type -------------------
    using state_type = int8_t; //must be signed
    const state_type max_state = 4;
    const uint n_neighbour = 6;
    const uint n_state = max_state;
    
    //------------------- technical typedefs -------------------
    using index_type = int;
}//end mc_namespace 

#endif //__GLOBAL_HEADER
