// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    25.03.2014 13:04:36 CET
// File:    result_struct.hpp

#ifndef __RESULT_STRUCT_HEADER
#define __RESULT_STRUCT_HEADER

namespace mc_potts {
  
    struct result_struct {
        result_struct(double m = 0, double d = 0, double e = 0, uint64_t _n = 0): mean(m), dev(d), err(e), n(_n) {
        }
        double mean;
        double dev;
        double err;
        uint64_t n;
    };
    
} // namespace mc_potts

#endif //__RESULT_STRUCT_HEADER
