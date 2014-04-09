// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    09.04.2014 10:17:27 CEST
// File:    types.hpp

#ifndef __MC_POTTS_TYPES_HEADER
#define __MC_POTTS_TYPES_HEADER

#include <cstdint>

namespace mc_potts {
    
    using index_type = uint16_t;
    using spin_ret_type = uint8_t;
    using seed_type = uint32_t;
    
    struct result_struct {
        result_struct(double m = 0, double d = 0, double e = 0, uint64_t _n = 0): mean(m), dev(d), err(e), n(_n) {
        }
        double mean;
        double dev;
        double err;
        uint64_t n;
    };
    
    
    
}//end namespace mc_potts

#endif //__MC_POTTS_TYPES_HEADER
