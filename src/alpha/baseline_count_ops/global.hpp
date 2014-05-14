// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    21.03.2014 09:57:00 CET
// File:    global.hpp

#ifndef __COUNT_OPS_GLOBAL_HEADER
#define __COUNT_OPS_GLOBAL_HEADER

#include <addon/debug.hpp>
#include <addon/accum.hpp>

//  +---------------------------------------------------+
//  |                   typedefs                        |
//  +---------------------------------------------------+
namespace mc_potts {
    namespace baseline_count_ops {
        //------------------- state type -------------------
        using state_type = int8_t; //must be signed
        const state_type max_state = 4;
        const uint n_neighbour = 6;
        const uint n_state = max_state;
        
        //------------------- technical typedefs -------------------
        using index_type = int;
        
    }//end namespace baseline_count_ops
}//end mc_namespace 

accum_class<int> flops;
accum_class<int> dops;
accum_class<int> iops;

#endif //__COUNT_OPS_GLOBAL_HEADER
