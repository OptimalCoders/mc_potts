// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    26.03.2014 20:52:50 CET
// File:    system.hpp

#ifndef __SYSTEM_BASELINE_GRESCHD_HEADER
#define __SYSTEM_BASELINE_GRESCHD_HEADER


#include <alpha/baseline_impl_greschd/global.hpp>
#include <addon/ppm_picture.hpp>

#include <vector>
#include <algorithm>
#include <cassert>
#include <math.h>

namespace mc_potts {
    
    namespace baseline_greschd {

        template<int L1, int L2, int L3, int S>
        class system_cl {
        //------------------------local typedefs----------------------------//
            typedef int spin_t;
            typedef uint8_t spin_rep_t;
            typedef std::vector<std::vector<std::vector<spin_rep_t>>> system_t;
        
        public:
        //------------------------public typedefs---------------------------//
            typedef int dim_t;
            
        //------------------------Constructor-------------------------------//
            system_cl(): system_(std::vector<std::vector<std::vector<spin_rep_t>>>
                        (L1, std::vector<std::vector<spin_rep_t>>
                        (L2, std::vector<spin_rep_t>(L3)))) {};
        
        //------------------------get - set---------------------------------//
            
            spin_t get(dim_t const & x, dim_t const & y, dim_t const & z) const {
                return system_.at(x).at(y).at(z);
            }
            
            spin_t get_nn(dim_t const & x, dim_t const & y, dim_t const & z) const {
                return  get((x - 1 + L1) % L1, y, z) + get((x + 1) % L1, y, z) +
                        get(x, (y - 1 + L2) % L2, z) + get(x, (y + 1) % L2, z) +
                        get(x, y, (z - 1 + L3) % L3) + get(x, y, (z + 1) % L3);
            }
            
            void set(dim_t const & x, dim_t const & y, dim_t const & z, spin_t const & s) {
                assert(abs(s) < S);
                system_.at(x).at(y).at(z) = spin_rep_t(s);
            }
            
        //------------------------observables-------------------------------//
        
            double magn_density() const {
                spin_t res = 0;
                for(dim_t i = 0; i < L1; ++i) {
                    for(dim_t j = 0; j < L2; ++j) {
                        for(dim_t k = 0; k < L3; ++k) {
                            res += get(i, j, k);
                        }
                    }
                }
                return res / (L1 * L2 * L3) - (S - 1) / 2.;
            }
            
            energy_t energy_density() const {
                energy_t res = 0;
                for(dim_t i = 0; i < L1; ++i) {
                    for(dim_t j = 0; j < L2; ++j) {
                        for(dim_t k = 0; k < L3; ++k) {
                            res -= (get(i, j, k) - (S - 1) / 2.) * (get_nn(i, j, k) - 6 * (S - 1) / 2. );
                        }
                    }
                }
                return res * Jh / (2. * L1 * L2 * L3);
            }
            
        //-------------------print a picture of a slice at x----------------//
            void picture_slice (dim_t const & x, std::string const & outfile = "output", dim_t resolution = 10) const {
                addon::ppm_picture_class pict(outfile);
                pict.set_cellsize(resolution);
                pict.init(S);
                pict.print(system_.at(x));
            }

        private:
        //------------------------private members---------------------------//
            system_t system_;
            
        };
    } // namespace baseline_greschd 
} // namespace mc_potts


#endif //__SYSTEM_BASELINE_GRESCHD_HEADER
