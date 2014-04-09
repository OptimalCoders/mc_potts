// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    09.04.2014 12:32:03 CEST
// File:    global.hpp

#ifndef __MC_POTTS_GLOBAL_HEADER
#define __MC_POTTS_GLOBAL_HEADER

#include <cstdint>

namespace mc_potts {
    int const S = 4; // S-state potts model
    uint8_t const n_neighbour = 6; //amount of neighbours for 3D
}//end namespace mc_potts

#endif //__MC_POTTS_GLOBAL_HEADER
