// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    21.03.2014 09:55:10 CET
// File:    sim.hpp

#ifndef __MSK_V01_SIM_HEADER
#define __MSK_V01_SIM_HEADER

#include <types.hpp>
#include <global.hpp>
#include <addon/color.hpp>
#include <addon/accum.hpp>
#include <alpha/msk_v01/grid.hpp>

#include <map>

namespace mc_potts {
    namespace msk_v1 {
        struct sim {
            template< int L1
                    , int L2
                    , int L3
                    , template<typename> class RNG
                    , class GRID = void
                    , class MATRIX = void >
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
                    assert(max_state == S);
                    clear();
                }
                void step() {
                    int const i = rngL1_();
                    int const j = rngL2_();
                    int const k = rngL3_();
                    int shift = rngUD_();
                    
                    auto & a = grid_.get(i, j, k);
                    
                    if(shift == 0) {
                        if(a == 0)
                            return;
                    }
                    else {
                        if(a == max_state - 1)
                            return;
                    }
                    
                    auto n = grid_.neighbour(i, j, k);
                    
                    if(pre_exp_[2*n + shift] > rngp_()) {
                        shift = shift * 2 - 1;
                        E_ += shift * (n - n_neighbour * ((double)S - 1) / 2);
                        a += shift;
                        M_ += shift;
                        accacc_ << 1;
                    }
                    else
                        accacc_ << 0;
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
                    assert(E_ == grid_.energy());
                    assert(M_ == grid_.magn());
                    
                    data_.at("E") << E_ / (L1 * L2 * L3);
                    data_.at("M") << M_ / (L1 * L2 * L3);
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
                    for(int i = 0; i < pre_exp_.size(); ++i) {
                        auto shift = 1 - ((i & 1) == 0)*2;
                        int n = i/2;
                        pre_exp_[i] = std::min(1.0, std::exp(eunit_ / T_ * shift * (n - 6 * (S - 1) / 2.0)));
                    }
                }
                void clear() {
                    data_.clear();
                    data_["E"];
                    data_["M"];
                    accacc_.clear();
                    
                    set_T(T_);
                    
                    grid_.init();
                    
                    RNG<state_type> rngS(0, S);
                    for(index_type i = 0; i < L1; ++i) {
                        for(index_type j = 0; j < L2; ++j) {
                            for(index_type k = 0; k < L3; ++k) {
                                grid_.get(i, j, k) = rngS();
                            }
                        }
                    }
                    
                    E_ = grid_.energy();
                    M_ = grid_.magn();
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
                    grid_.print();
                    std::cout << GREEN << "Probabilities: " << NONE << std::endl;
                    for(auto & a: pre_exp_) {
                        std::cout << a << " ";
                    }
                    std::cout << std::endl;
                    std::cout << GREEN << "Measurements: " << NONE << std::endl;
                    for(auto & d : data_)
                        std::cout << d.first << ": " << d.second << std::endl;
                }
                
            private:
                //------------------- grid -------------------
                grid_class<L1, L2, L3> grid_;
                
                //------------------- measurements/physics -------------------
                std::map<std::string, accum_class<double>> data_;
                accum_class<int> accacc_;
                double const eunit_;
                double T_;
                double E_;
                double M_;
                
                uint32_t const N_therm_;
                uint32_t const N_update_;
                
                //------------------- technical -------------------
                std::array<double, 2*(n_neighbour * (max_state - 1) + 1)> pre_exp_;
                
                //------------------- rngs -------------------
                RNG<index_type> rngL1_;
                RNG<index_type> rngL2_;
                RNG<index_type> rngL3_;
                RNG<double> rngp_;
                RNG<int> rngUD_;
            };
        };
    }//end namespace msk_v1
}//end namespace mc_potts

#endif //__MSK_V01_SIM_HEADER
