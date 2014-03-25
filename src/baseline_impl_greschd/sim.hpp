// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    24.03.2014 21:00:38 CET
// File:    sim_basline_greschd.hpp

#ifndef __SIM_BASLINE_GRESCHD_HEADER
#define __SIM_BASLINE_GRESCHD_HEADER

#include <baseline_impl_greschd/lib/sim.cpp>

namespace mc_potts {
    
    struct result_struct;

    struct sim_baseline_greschd /* {

    template< int L1
            , int L2
            , int L3
            , int S
            , template<class> class RNG >
    class impl {
    public:
        sim_baseline_greschd(double const & T_init, uint32_t const & N_therm, uint32_t const & N_update = L1*L2*L3);
        
        void thermalize();
        
        void update();
        
        void measure();
        
        result_struct energy() const;
        
        result_struct magn() const;
        
        void set_T(double const & T);
        
        void clear();
        
        uint8_t get(  uint32_t const & l1
                    , uint32_t const & l2
                    , uint32_t const & l3) const;
                    
    }; // impl

    }*/; // struct sim_baseline_greschd
    
} // namespace mc_potts

#endif //__SIM_BASLINE_GRESCHD_HEADER
