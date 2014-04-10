// Author:  Dominik Gresch <greschd@ethz.ch>
// Date:    10.04.2014 01:11:35 CEST
// File:    baseline_greschd_impl.hpp

#ifndef __BASELINE_GRESCHD_impl_HEADER
#define __BASELINE_GRESCHD_impl_HEADER


#include <alpha/baseline_impl_greschd/global.hpp>

#include <vector>
#include <algorithm>
#include <cassert>
#include <math.h>

namespace mc_potts {
    
    struct baseline_greschd_grid {
        
        template<int L1, int L2, int L3, typename MATRIX>
        class impl {
        //------------------------local typedefs----------------------------//
            typedef uint16_t spin_rep_t;
            typedef std::vector<std::vector<std::vector<spin_rep_t>>> system_t;
        
        public:
        //------------------------public typedefs---------------------------//
            typedef int index_type;
            
        //------------------------Constructor-------------------------------//
            impl(): system_() {};
        
        //------------------------get - set---------------------------------//
            
            inline spin_ret_type const & get(index_type const & x, index_type const & y, index_type const & z) const {
                return system_.get(x, y, z);
            }
            
            spin_ret_type get_nn(index_type const & x, index_type const & y, index_type const & z) const {
                return  get((x - 1 + L1) % L1, y, z) + get((x + 1) % L1, y, z) +
                        get(x, (y - 1 + L2) % L2, z) + get(x, (y + 1) % L2, z) +
                        get(x, y, (z - 1 + L3) % L3) + get(x, y, (z + 1) % L3);
            }
            
            void set(index_type const & x, index_type const & y, index_type const & z, spin_ret_type const & s) {
                system_.set(x, y, z, s);
            }
            
        //------------------------observables-------------------------------//
        
            double magn_density() const {
                spin_ret_type res = 0;
                for(index_type i = 0; i < L1; ++i) {
                    for(index_type j = 0; j < L2; ++j) {
                        for(index_type k = 0; k < L3; ++k) {
                            res += get(i, j, k);
                        }
                    }
                }
                return res / (L1 * L2 * L3) - (S - 1) / 2.;
            }
            
            double energy_density() const {
                double res = 0;
                for(index_type i = 0; i < L1; ++i) {
                    for(index_type j = 0; j < L2; ++j) {
                        for(index_type k = 0; k < L3; ++k) {
                            res -= (get(i, j, k) - (S - 1) / 2.) * (get_nn(i, j, k) - 6 * (S - 1) / 2. );
                        }
                    }
                }
                return res * baseline_greschd::Jh / (2. * L1 * L2 * L3);
            }
            

        private:
        //------------------------private members---------------------------//
            typename MATRIX::template impl<L1, L2, L3> system_;
            
        };
        static std::string name() {
            return "baseline_greschd_grid";
        }
    }; // struct baseline_greschd_impl 
    
} // namespace mc_potts


#endif //__BASELINE_GRESCHD_impl_HEADER
