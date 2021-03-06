// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    14.05.2014 21:38:20 CEST
// File:    msk_v0_sim.hpp

#ifndef __MSK_V0_SIM_HEADER
#define __MSK_V0_SIM_HEADER

#include <assert.h>
#include <types.hpp>
#include <global.hpp>
#include <addon/color.hpp>
#include <addon/accum.hpp>
#include <addon/micro_bench.hpp>

#include <map>

namespace mc_potts {
        //------------------- technical typedefs -------------------
        struct msk_v0_sim {
            template< int L1
                    , int L2
                    , int L3
                    , template<typename> class RNG
                    , class GRID
                    , class MATRIX>
            class impl {
            public:
                impl(double const & T_init, uint32_t const & N_therm, uint32_t const & N_update = L1 * L2 * L3): 
                                     eunit_(1)
                                   , T_(10)
                                   , N_therm_(N_therm)
                                   , N_update_(N_update)
                                   , rngL1_(0, L1)
                                   , rngL2_(0, L2)
                                   , rngL3_(0, L3)
                                   , rngp_()
                                   , rngUD_(0, 2) {
                    assert(S == S);
                    clear();
                }
                void step() {
                    START_MICRO("step")
                    START_MICRO("rng")
                    int const i = rngL1_();
                    int const j = rngL2_();
                    int const k = rngL3_();
                    int shift = rngUD_();
                    
                    NEXT_MICRO("spin_access")
                    auto a = grid_.get(i, j, k);
                    
                    if(shift == 0) {
                        if(a == 0) {
                            STOP_MICRO()
                            STOP_MICRO()
                            return;
                        }
                    }
                    else {
                        if(a == S - 1) {
                            STOP_MICRO()
                            STOP_MICRO()
                            return;
                        }
                    }
                    NEXT_MICRO("stencil")
                    
                    auto n = grid_.get_nn(i, j, k);
                    
                    NEXT_MICRO("metropolis")
                    shift = 2 * shift - 1;
                    if(std::min(1.0, std::exp(eunit_ / T_ * shift * (n - 6 * (S - 1) / 2.0))) > rngp_()) {
                        grid_.set(i, j, k, a + shift);
                    }
                    STOP_MICRO()
                    STOP_MICRO()
                }
                void update() { //ensure decorr
                    for(uint32_t i = 0; i < N_update_; ++i) {
                        step();
                    }
                }
                void thermalize() {
                    for(uint32_t i = 0; i < N_therm_; ++i) {
                        update();
                    }
                }
                void measure() { //feeding
                    E_ = calc_energy();
                    M_ = calc_magn();
                    
                    assert(E_ == calc_energy());
                    assert(M_ == calc_magn());
                    
                    data_.at("E") << E_ / (L1 * L2 * L3);
                    data_.at("M") << M_ / (L1 * L2 * L3);
                }
                double calc_energy() const {
                    double E = 0;
                    for(index_type i = 0; i < L1; ++i) {
                        for(index_type j = 0; j < L2; ++j) {
                            for(index_type k = 0; k < L3; ++k)
                                E += (grid_.get(i, j, k) - (S - 1) / 2.0)
                                   * (grid_.get_nn(i, j, k) - n_neighbour * (S - 1) / 2.0);
                        }
                    }
                    return E / 2;
                }
                double calc_magn() const {
                    double M = 0;
                    for(index_type i = 0; i < L1; ++i)
                        for(index_type j = 0; j < L2; ++j)
                            for(index_type k = 0; k < L3; ++k)
                                M += grid_.get(i, j, k);
                    return M;
                }
                mc_potts::result_struct energy() const {
                    return mc_potts::result_struct(data_.at("E").mean()
                                                 , data_.at("E").deviation()
                                                 , data_.at("E").error()
                                                 , data_.at("E").count()
                                                 );
                }
                mc_potts::result_struct magn() const {
                    return mc_potts::result_struct(data_.at("M").mean()
                                                 , data_.at("M").deviation()
                                                 , data_.at("M").error()
                                                 , data_.at("M").count()
                                                 );
                }
                void set_T(double const & T) {
                    T_ = T;
                }
                void clear() {
                    data_.clear();
                    data_["E"];
                    data_["M"];
                    accacc_.clear();
                    
                    set_T(T_);
                    
                    //~ grid_.init();
                    
                    RNG<spin_ret_type> rngS(0, S);
                    for(index_type i = 0; i < L1; ++i) {
                        for(index_type j = 0; j < L2; ++j) {
                            for(index_type k = 0; k < L3; ++k) {
                                grid_.set(i, j, k, rngS());
                            }
                        }
                    }
                    
                    E_ = calc_energy();
                    M_ = calc_magn();
                }
                uint8_t get(  uint32_t const & l1
                            , uint32_t const & l2
                            , uint32_t const & l3) const {
                    return grid_.get(l1, l2, l3);
                }
                //  +---------------------------------------------------+
                //  |                   const methods                   |
                //  +---------------------------------------------------+
                //------------------- print -------------------
                void print() const {
                    std::cout << GREEN << "Grid: " << NONE << std::endl;
                    //~ grid_.print();
                    std::cout << GREEN << "Measurements: " << NONE << std::endl;
                    for(auto & d : data_)
                        std::cout << d.first << ": " << d.second << std::endl;
                }
                static std::string spec() {
                    return name() + ", " + GRID::name() + ", " + MATRIX::name() + ", " + RNG<int>::name();
                }
            private:
                //------------------- grid -------------------
                typename GRID::template impl<L1, L2, L3, MATRIX> grid_;
                
                //------------------- measurements/physics -------------------
                std::map<std::string, accum_class<double>> data_;
                accum_class<int> accacc_;
                double const eunit_;
                double T_;
                double E_;
                double M_;
                
                uint32_t const N_therm_;
                uint32_t const N_update_;
                
                //------------------- rngs -------------------
                RNG<index_type> rngL1_;
                RNG<index_type> rngL2_;
                RNG<index_type> rngL3_;
                RNG<double> rngp_;
                RNG<int> rngUD_;
            };
            static std::string name() {
                return "msk_v0_sim";
            }
        };
}//end namespace mc_potts

#endif //__MSK_V0_SIM_HEADER
