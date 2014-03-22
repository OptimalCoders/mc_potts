// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    22.03.2014 18:59:07 CET
// File:    global.hpp

#ifndef __GLOBAL_HEADER
#define __GLOBAL_HEADER

#ifndef SEED
#define SEED 42
#endif

namespace mc_potts {
    typedef int num_t;
    typedef int dim_t;
    typedef double spin_t;
    typedef double prob_t;
    typedef double energy_t;
    typedef std::mt19937 rng_t;
    
    
    rng_t rng(SEED);
    
    // J * hbar 
    const energy_t Jh = 1;
    const energy_t kb = 1;
    const energy_t physical_const = Jh / kb;
}

#endif //__GLOBAL_HEADER
