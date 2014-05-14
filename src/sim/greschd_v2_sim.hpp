// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    26.03.2014 20:46:58 CET
// File:    sim.hpp

// features: 
// at-the-end calculation of E & M
// probability lookup


#ifndef __GRESCHD_V2_SIM_HEADER
#define __GRESCHD_V2_SIM_HEADER

#include <types.hpp>
#include <global.hpp>
#include <alpha/baseline_impl_greschd/global.hpp>
#include <algorithm>
#include <vector>
#include <math.h>

namespace mc_potts {
    
    struct greschd_v2_sim {
        
        template< int L1
                , int L2
                , int L3
                , template<typename> class RNG
                , class GRID
                , class MATRIX>
        class impl {
            //---------------------local typedefs---------------------------//
            typedef int dir_t;
            typedef std::vector<double> resvec_t;
            
        public:
            impl(   double const & T_init,
                    uint32_t const & N_therm, 
                    uint32_t const & N_update = L1*L2*L3):  
                        N_therm_(N_therm), 
                        N_update_(N_update), 
                        T_(T_init), 
                        system_(), 
                        energy_res_(0),
                        magn_res_(0),
                        rng1_(0, L1),
                        rng2_(0, L2),
                        rng3_(0, L3),
                        rngdir_(),
                        rngprob_() {
                clear();
                prob_ = (double*)malloc((6 * (S - 1) + 1) *sizeof(double));
                prob_update_();
                
                idx1 = rng1_();
                idx2 = rng2_();
                idx3 = rng3_();
                dir1 = rngdir_();
                r1 = rngprob_(); 
            }
            
            
            // N_therm_ single spin updates
            void thermalize() {
                for(index_type i = 0; i < N_therm_; ++i) {
                    update();
                }
            }
            
            // N_update_ single spin updates
            void update() {
                
                index_type idx4;
                index_type idx5;
                index_type idx6;
                double p;
                double r2;
                dir_t dir2;
                spin_ret_type temp;
                
                
                for(index_type i = 0; i < N_update_ / 2; ++i) {
                    
                    idx4 = rng1_();
                    idx5 = rng2_();
                    idx6 = rng3_();
                    dir2 = rngdir_();
                    r2 = rngprob_(); 
                    
                    // choosing direction of the spin change
                    dir1 = dir1 * 2 - 1;
                    temp = system_.get(idx1, idx2, idx3) + dir1;
                    
                    //return if the new state isn't a valid state
                    if(not(temp >= S or temp < 0)) {
                        // acceptance step
                        p = prob_[dir1 * (system_.get_nn(idx1, idx2, idx3) - 3 * (S - 1)) + 3 * (S - 1)];
                        if(p > r1) {
                            system_.set(idx1, idx2, idx3, temp);
                        }
                    }
                    
                    
                    // second step
                    // generating random numbers for next first step
                    idx1 = rng1_();
                    idx2 = rng2_();
                    idx3 = rng3_();
                    dir1 = rngdir_();
                    r1 = rngprob_(); 
                    
                    // choosing direction of the spin change
                    dir2 = dir2 * 2 - 1;
                    temp = system_.get(idx4, idx5, idx6) + dir2;
                    
                    //return if the new state isn't a valid state
                    if(not(temp >= S or temp < 0)) {
                        // acceptance step
                        p = prob_[dir2 * (system_.get_nn(idx4, idx5, idx6) - 3 * (S - 1)) + 3 * (S - 1)];
                        if(p > r2) {
                            system_.set(idx4, idx5, idx6, temp);
                        }
                    }
                    
                }
            }
            
            //
            void measure() {
                energy_res_.push_back(energy_density_());
                magn_res_.push_back(magn_density_());
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
                prob_update_();
            }
            
            void clear() {
                RNG<spin_ret_type> spin_rng(0, S);
                
                // set all spins to a random state
                for(index_type i = 0; i < L1; ++i) {
                    for(index_type j = 0; j < L2; ++j) {
                        for(index_type k = 0; k < L3; ++k) {
                            system_.set(i, j, k, spin_rng());
                        }
                    }
                }
            }
            
            uint8_t get(  uint32_t const & l1, uint32_t const & l2, uint32_t const & l3) const {
                return system_.get(l1, l2, l3); 
            }
            static std::string spec() {
                return name() + ", " + GRID::name() + ", " + MATRIX::name() + ", " + RNG<int>::name();
            }
        
        private:
        
    //------------------------update probabilities----------------------//
            void prob_update_() {
                for(uint i = 0; i <  6 * (S - 1) + 1; ++i) {
                    prob_[i] = exp(baseline_greschd::physical_const / T_  * (i - 6 * (S - 1) / 2.));
                    std::cout << prob_[i] << std::endl;
                }
            }
    
    //------------------------observables-------------------------------//
        
            double magn_density_() const {
                spin_ret_type res = 0;
                for(index_type i = 0; i < L1; ++i) {
                    for(index_type j = 0; j < L2; ++j) {
                        for(index_type k = 0; k < L3; ++k) {
                            res += system_.get(i, j, k);
                        }
                    }
                }
                return res / double(L1 * L2 * L3) - (S - 1) / 2.;
            }
            
            double energy_density_() const {
                double res = 0;
                for(index_type i = 0; i < L1; ++i) {
                    for(index_type j = 0; j < L2; ++j) {
                        for(index_type k = 0; k < L3; ++k) {
                            res -= (system_.get(i, j, k) - (S - 1) / 2.) * (system_.get_nn(i, j, k) - 6 * (S - 1) / 2. );
                        }
                    }
                }
                return res * baseline_greschd::Jh / (2. * L1 * L2 * L3);
            }
            
        //------------------------calculating the results from a vector-----//
            result_struct get_res_(resvec_t const & v) const {
                result_struct res;
                res.n = v.size();
                
                res.mean = std::accumulate(v.begin(), v.end(), 0.) / res.n;
                
                res.dev = 0;
                std::for_each(v.begin(), v.end(), [&](double x){res.dev += (x - res.mean)*(x - res.mean);});
                res.dev = sqrtf(res.dev / res.n);
                
                res.err = res.dev / sqrt(res.n);
                
                return res;
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
            double T_;
            
            // system
            typename GRID::template impl<L1, L2, L3, MATRIX> system_;
            
            // results
            resvec_t energy_res_;
            resvec_t magn_res_;
            
            // probability lookup table
            double *prob_;
            
            // RNG distributions
            RNG<index_type> rng1_;
            RNG<index_type> rng2_;
            RNG<index_type> rng3_;
            RNG<dir_t> rngdir_;
            RNG<double> rngprob_;
            
            // initial random numbers
            index_type idx1;
            index_type idx2;
            index_type idx3;
            dir_t dir1;
            double r1;
                
        }; // impl
        static std::string name() {
            return "greschd_v2_sim";
        }
    }; // struct greschd_v2_sim
    
} // namespace mc_potts


#endif //__GRESCHD_V2_SIM_HEADER
