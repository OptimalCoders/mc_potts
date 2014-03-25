// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    22.03.2014 18:19:50 CET
// File:    system.hpp

#ifndef __SYSTEM_HEADER
#define __SYSTEM_HEADER

#include <vector>
#include <algorithm>
#include <cassert>
#include <math.h>
#include "global.hpp"
#include "ppm_picture.hpp"

namespace mc_potts {

    template<int L1, int L2, int S>
    class system_cl {
    //------------------------local typedefs----------------------------//
        typedef int spin_rep_t;
        typedef std::vector<std::vector<spin_rep_t>> system_t;
    
    public:
    //------------------------public typedefs---------------------------//
        typedef int dim_t;
        
    //------------------------Constructor-------------------------------//
        system_cl(): system_(std::vector<std::vector<spin_rep_t>>(L1, std::vector<spin_rep_t>(L2))) {};
        
        
    //------------------------init - initialize with random state-------//
        void init() {
            std::uniform_int_distribution<spin_rep_t> distr(0, S - 1);
            for(auto & x: system_) {
                for(auto & y : x) {
                    y = distr(rng);
                }
            }
        }
    
    //------------------------get - set---------------------------------//
        
        spin_t get(dim_t const & x, dim_t const & y) const {
            return system_.at(x).at(y) - 1.5;
        }
        
        spin_t get_nn(dim_t const & x, dim_t const & y) const {
            return - 6  + system_.at((x - 1 + L1) % L1).at(y) 
                        + system_.at((x + 1) % L1).at(y) 
                        + system_.at(x).at((y - 1 + L2) % L2) 
                        + system_.at(x).at((y + 1) % L2);
        }
        
        void set(dim_t const & x, dim_t const & y, spin_t const & s) {
            // add 0.1 to ensure correct round - down
            assert(fabs(s) < ((S - 1)/2. + 0.1));
            
            system_.at(x).at(y) = spin_rep_t(s + 1.6);
        }
        
    //------------------------observables-------------------------------//
    
        spin_t magn_density() const {
            spin_t res = 0;
            for(dim_t i = 0; i < L1; ++i) {
                for(dim_t j = 0; j < L2; ++j) {
                    res += get(i, j);
                }
            }
            return res / (L1 * L2);
        }
        
        energy_t energy_density() const {
            energy_t res = 0;
            for(dim_t i = 0; i < L1; ++i) {
                for(dim_t j = 0; j < L2; ++j) {
                    res -= get(i, j) * get_nn(i, j);
                }
            }
            return res * Jh / (2. * L1 * L2);
        }
        
    //------------------------printing - output-------------------------//
    
        void print() const {
            for(dim_t i = 0; i < L1; ++i) {
                for(dim_t j = 0; j < L2; ++j) {
                    std::cout << get(i, j) << " ";
                }
                std::cout << std::endl;
            }
        }
        
        void picture(std::string const & outfile = "output", dim_t resolution = 10) const {
            addon::ppm_picture_class out(outfile);
            out.set_cellsize(resolution);
            out.init(S);
            out.print(system_);
        }
        
        

    private:
    //------------------------private members---------------------------//
        system_t system_;
        
    };

}

#endif //__SYSTEM_HEADER
