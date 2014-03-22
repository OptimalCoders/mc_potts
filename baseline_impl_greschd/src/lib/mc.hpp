// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    22.03.2014 18:52:49 CET
// File:    mc.hpp

#ifndef __MC_HEADER
#define __MC_HEADER

#include "system.hpp"
#include <random>
#include <math.h>

namespace mc_potts {
    
    template<int L1, int L2, int S>
    class mc_cl {
    //------------------------private typedefs--------------------------//
        typedef std::uniform_int_distribution<int> discrete_distribution_t;
        typedef std::uniform_real_distribution<double> continuous_distribution_t;

        
    public:
    //------------------------public typedefs---------------------------//
        typedef double prob_t;
        typedef int seed_t;
        
    //------------------------Constructor-------------------------------//
        mc_cl(energy_t T): system_(), T_(T), xpos_dist_(0, L1 - 1), ypos_dist_(0, L2 - 1), prob_dist_(0., 1.), bin_dist_(0, 1), max_spin_((S - 1) / 2.) {
                system_.init();
            };
            
    //------------------------change T----------------------------------//
    
        void set_T(energy_t T) {
            T_ = T;
        }
        
    //------------------------single-spin update------------------------//
        void update_spin() {
        
            // choosing a spin
            dim_t i = xpos_dist_(rng);
            dim_t j = ypos_dist_(rng);
            
            // choosing direction of the spin change
            spin_t direction = bin_dist_(rng);
            direction = direction * 2 - 1;
            
            spin_t temp = system_.get(i, j) + direction;
            
            //return if the new state isn't a valid state
            if(fabs(temp) > max_spin_) {
                return;
            }
        
            // acceptance step
            prob_t p = exp(physical_const / T_ * direction * system_.get_nn(i, j));
            prob_t r = prob_dist_(rng);
            if(p > r) {
                system_.set(i, j, temp);
            }
            return;
        }
    
    //------------------------multiple updates--------------------------//
        void update(num_t const & num_runs) {
            for(num_t i = 0; i < num_runs; ++i) {
                update_spin();
            }
            
        }
        
    //------------------------output - plots----------------------------//
        
        void print() const {
            std::cout << "system:" << std::endl;
            system_.print();
        }
        
        void print_observables() const {
            std::cout << "energy density: " << system_.energy_density() << std::endl;
            std::cout << "magnetisation density: " << system_.magn_density() << std::endl;
        }
        
        void picture(std::string const & outfile = "output") {
            system_.picture(outfile);
        }
        
    private:
        system_cl<L1, L2, S> system_;
        energy_t T_;
        
        discrete_distribution_t xpos_dist_;
        discrete_distribution_t ypos_dist_;
        continuous_distribution_t prob_dist_;
        discrete_distribution_t bin_dist_;
        
        const spin_t max_spin_;
    };
}

#endif //__MC_HEADER
