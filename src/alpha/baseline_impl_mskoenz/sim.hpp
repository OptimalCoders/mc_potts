// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    21.03.2014 09:55:10 CET
// File:    sim.hpp

#ifndef __BASELINE_IMPL_MSKOENZ_SIM_HEADER
#define __BASELINE_IMPL_MSKOENZ_SIM_HEADER

#include <types.hpp>
#include <global.hpp>
#include <addon/color.hpp>
#include <addon/accum.hpp>
#include <alpha/baseline_impl_mskoenz/grid.hpp>

#include <map>

namespace mc_potts {
    namespace baseline_mskoenz {
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
                    
                    auto & a = grid_.ref(i, j, k);
                    
                    if(shift == 0) {
                        if(a == 0)
                            return;
                    }
                    else {
                        if(a == max_state - 1)
                            return;
                    }
                    
                    auto n = grid_.neighbour(i, j, k);
                    
                    //~ auto const E_new = grid_.neighbour(i, j, k);
                    //~ auto const index = (n_neighbour + E_old - E_new) >> 1;
                    
                    //~ assert(std::abs(E_old - E_new) <= n_neighbour);
                    //~ assert(std::abs(E_old - E_new) % 2 == 0);
                    
                    double prob = pre_exp_[2*n + shift];
                    shift = shift * 2 - 1;
                    
                    //~ if(pre_exp_[n] > rngp_()) {
                    if(prob > rngp_()) {
                        E_ += shift * (n - n_neighbour * ((double)S - 1) / 2);
                        a += shift;
                        M_ += shift;
                        data_.at("acc") << 1;
                    }
                    else
                        data_.at("acc") << 0;
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
                    data_["acc"];
                    data_["E"];
                    data_["M"];
                    
                    set_T(T_);
                    
                    grid_.init();
                    
                    RNG<state_type> rngS(0, S);
                    for(index_type i = 0; i < L1; ++i) {
                        for(index_type j = 0; j < L2; ++j) {
                            for(index_type k = 0; k < L3; ++k) {
                                grid_.ref(i, j, k) = rngS();
                            }
                        }
                    }
                    
                    E_ = grid_.energy();
                    M_ = grid_.magn();
                }
                uint8_t get(  uint32_t const & l1
                            , uint32_t const & l2
                            , uint32_t const & l3) const {
                    return grid_.grid()[l1][l2][l3];
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
    }//end namespace baseline_mskoenz
}//end namespace mc_potts

#endif //__BASELINE_IMPL_MSKOENZ_SIM_HEADER
