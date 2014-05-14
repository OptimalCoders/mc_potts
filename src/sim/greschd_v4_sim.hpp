// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    26.03.2014 20:46:58 CET
// File:    sim.hpp

// features: 
// at-the-end calculation of E & M
// probability lookup


#ifndef __GRESCHD_V4_SIM_HEADER
#define __GRESCHD_V4_SIM_HEADER

#include <types.hpp>
#include <global.hpp>
#include <alpha/baseline_impl_greschd/global.hpp>

#include <vector>
#include <math.h>
#include <algorithm>

namespace mc_potts {
    
    struct greschd_v4_sim {
        
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
                temp1 = system_.get(idx1, idx2, idx3) + dir1;
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
                spin_ret_type temp2;
                
                #define RNG(IDX1, IDX2, IDX3, DIR, R, TEMP)             \
                IDX1 = rng1_();                                         \
                IDX2 = rng2_();                                         \
                IDX3 = rng3_();                                         \
                DIR = rngdir_();                                        \
                R = rngprob_();                                         \
                TEMP = system_.get(IDX1, IDX2, IDX3);                   // RNG
                
                #define STEP(IDX1, IDX2, IDX3, IDX4, IDX5, IDX6, DIR, R, TEMP, TEMP2)                   \
                DIR = DIR * 2 - 1;                                                                      \
                TEMP += DIR;                                                                            \
                if(not(TEMP >= S or TEMP < 0)) {                                                        \
                    p = prob_[DIR * (system_.get_nn(IDX1, IDX2, IDX3) - 3 * (S - 1)) + 3 * (S - 1)];    \
                    if(p > R) {                                                                         \
                        system_.set(IDX1, IDX2, IDX3, TEMP);                                            \
                        if(IDX1 == IDX4 and IDX2 == IDX5 and IDX3 == IDX6) {                            \
                            TEMP2 = TEMP;                                                               \
                        }                                                                               \
                    }                                                                                   \
                }                                                                                       // STEP
                
                
                
                for(index_type i = 0; i < N_update_ / 2; ++i) {
                    
                    RNG(idx4, idx5, idx6, dir2, r2, temp2)
                    STEP(idx1, idx2, idx3, idx4, idx5, idx6, dir1, r1, temp1, temp2)
                    
                    RNG(idx1, idx2, idx3, dir1, r1, temp1)
                    STEP(idx4, idx5, idx6, idx1, idx2, idx3, dir2, r2, temp2, temp1)
                    
                }
                
                #undef RNG
                #undef STEP
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
            spin_ret_type temp1;
                
        }; // impl
        static std::string name() {
            return "greschd_v4_sim";
        }
    }; // struct greschd_v4_sim
    
} // namespace mc_potts


#endif //__GRESCHD_V4_SIM_HEADER
