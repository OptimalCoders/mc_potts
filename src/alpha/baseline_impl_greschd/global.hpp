// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    26.03.2014 20:51:53 CET
// File:    global.hpp

#ifndef __GLOBAL_BASELINE_GRESCHD_HEADER
#define __GLOBAL_BASELINE_GRESCHD_HEADER

namespace mc_potts {
    
    namespace baseline_greschd {

        typedef int num_t;
        typedef int dim_t;
        typedef uint8_t spin_t;
        typedef double prob_t;
        typedef double energy_t;
        
        // J * hbar 
        const energy_t Jh = 1;
        const energy_t kb = 1;
        const energy_t physical_const = Jh / kb;
        
    } // namespace baseline_greschd 
} // namespace mc_potts


#endif //__GLOBAL_BASELINE_GRESCHD_HEADER
