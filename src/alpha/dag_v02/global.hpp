// Author:  Dominik Gresch <greschd@ethz.ch>
// Date:    26.03.2014 23:16:32 CET
// File:    global.hpp

#ifndef __GLOBAL_V02_HEADER
#define __GLOBAL_V02_HEADER

namespace mc_potts {
    namespace dag_v02 {
        typedef int num_t;
        typedef int dim_t;
        typedef uint8_t spin_t;
        typedef double prob_t;
        typedef double energy_t;
        
        // J * hbar 
        const energy_t Jh = 1;
        const energy_t kb = 1;
        const energy_t physical_const = Jh / kb;
    } // namespace v1
}


#endif //__GLOBAL_V02_HEADER
