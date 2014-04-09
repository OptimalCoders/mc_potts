// Author:  Dominik Gresch <greschd@ethz.ch>
// Date:    26.03.2014 23:15:19 CET
// File:    sim_v1.hpp

#ifndef __SIM_V1_HEADER
#define __SIM_V1_HEADER


#include <v1_int2t/system.hpp>
#include <types.hpp>

#include <algorithm>
#include <vector>
#include <math.h>

namespace mc_potts {
    
    namespace v1_int2t {

        struct sim {

        template< int L1
                , int L2
                , int L3
                , int S
                , template<class> class RNG >
        class impl {
            //---------------------local typedefs---------------------------//
            typedef uint8_t spin_t;
            typedef int dim_t;
            typedef double prob_t;
            typedef double energy_t;
            typedef double res_t;
            typedef std::vector<res_t> resvec_t;
            
        public:
            impl(   double const & T_init,
                    uint32_t const & N_therm, 
                    uint32_t const & N_update = L1*L2*L3):  
                        N_therm_(N_therm), 
                        N_update_(N_update), 
                        T_(T_init), 
                        system_(), 
                        energy_res_(0),
                        magn_res_(0) {
                clear();
            }
            
            
            // N_therm_ single spin updates
            void thermalize() {
                for(dim_t i = 0; i < N_therm_; ++i) {
                    update();
                }
            }
            
            // N_update_ single spin updates
            void update() {
                for(dim_t i = 0; i < N_update_; ++i) {
                    update_spin_();
                }
            }
            
            //
            void measure() {
                energy_res_.push_back(system_.energy_density());
                magn_res_.push_back(system_.magn_density());
            }
            
            result_struct energy() const {
                return get_res_(energy_res_);
            }
            
            result_struct magn() const {
                return get_res_(magn_res_);
            }
            
            void set_T(double const & T) {
                clear_res_();
                T_ = T;
            }
            
            void clear() {
                RNG<spin_t> spin_rng(0, S);
                
                // set all spins to a random state
                for(dim_t i = 0; i < L1; ++i) {
                    for(dim_t j = 0; j < L2; ++j) {
                        for(dim_t k = 0; k < L3; ++k) {
                            system_.set(i, j, k, spin_rng());
                        }
                    }
                }
                
            }
            
            uint8_t get(  uint32_t const & l1, uint32_t const & l2, uint32_t const & l3) const {
                return system_.get(l1, l2, l3); 
            }
            
            //~ void picture_slice (dim_t const & x, std::string const & outfile = "output", dim_t resolution = 10) const {
                //~ assert(x < L1);
                //~ system_.picture_slice(x, outfile, resolution);
            //~ }
            
        private:
        
        //------------------------calculating the results from a vector-----//
            result_struct get_res_(resvec_t const & v) const {
                result_struct res;
                res.n = v.size();
                
                res.mean = std::accumulate(v.begin(), v.end(), 0.) / res.n;
                
                res.dev = 0;
                std::for_each(v.begin(), v.end(), [&](res_t x){res.dev += (x - res.mean)*(x - res.mean);});
                res.dev = sqrtf(res.dev / res.n);
                
                res.err = res.dev / sqrt(res.n);
                
                return res;
            }
        
            
        //------------------------single-spin update------------------------//
            void update_spin_() {
                
                RNG<dim_t> rng1(0, L1);
                RNG<dim_t> rng2(0, L2);
                RNG<dim_t> rng3(0, L3);
                RNG<dim_t> rngdir;
                RNG<prob_t> rngprob;
                
                dim_t i = rng1();
                dim_t j = rng2();
                dim_t k = rng3();
                
                // choosing direction of the spin change
                dim_t dir = rngdir();
                dir = dir * 2 - 1;
                spin_t temp = system_.get(i, j, k) + dir;
                
                //return if the new state isn't a valid state
                if(temp >= S or temp < 0) {
                    return;
                }
                
                // acceptance step
                prob_t p = exp(physical_const / T_ * dir * (system_.get_nn(i, j, k) - 6 * (S - 1) / 2.));
                prob_t r = rngprob();
                if(p > r) {
                    system_.set(i, j, k, temp);
                }
            }
            
        //---------------------------clear results--------------------------//
            void clear_res_() {
                energy_res_ = resvec_t(0);
                magn_res_ = resvec_t(0);
            }
            
            
        //---------------------------private variables----------------------//
            
            // num_runs
            uint32_t N_therm_;
            uint32_t N_update_;
            
            // temperature
            energy_t T_;
            
            // system
            system_cl<L1, L2, L3, S> system_;
            
            // results
            resvec_t energy_res_;
            resvec_t magn_res_;
            
        }; // impl

        }; // struct sim_v1
        
    } // namespace v1
} // namespace mc_potts


#endif //__SIM_V1_HEADER
