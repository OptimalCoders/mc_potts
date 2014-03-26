// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    21.03.2014 09:55:10 CET
// File:    sim.hpp

#ifndef __SIM_HEADER
#define __SIM_HEADER

#include <result.hpp>
#include <addon/color.hpp>
#include <addon/accum_double.hpp>
#include <baseline_impl_mskoenz/grid.hpp>
#include <baseline_impl_mskoenz/random2_msk.hpp>

#include <map>

namespace mc_potts {
    struct baseline_mskoenz_struct {
        template< int L1
                , int L2
                , int L3
                , int S
                , template<typename> class RNG >
        class impl {
        public:
            impl(double const & T_init, uint32_t const & N_therm, uint32_t const & N_update): 
                                 eunit_(1)
                               , T_(10)
                               , N_therm_(N_therm)
                               , N_update_(N_update)
                               , rngL1_(0, L1)
                               , rngL2_(0, L2)
                               , rngL3_(0, L3)
                               , rngp_()
                               , rngS_(0, 2) {
                clear();
            }
            void step() {
                int const i = rngL1_();
                int const j = rngL2_();
                int const k = rngL3_();
                int const shift = rngS_();
                
                auto & a = grid_.ref(i, j, k);
                auto const old = a;
                auto const E_old = grid_.neighbour_diff(i, j, k);
                
                if(shift == 0)
                    if(a > 0)
                        --a;
                    else
                        return;
                else
                    if(a + 1 < max_state)
                        ++a;
                    else
                        return;
                        
                auto const E_new = grid_.neighbour_diff(i, j, k);
                auto const index = (n_neighbour + E_old - E_new) >> 1;
                
                assert(std::abs(E_old - E_new) <= n_neighbour);
                assert(std::abs(E_old - E_new) % 2 == 0);
                
                if(pre_exp_[index] > rngp_()) {
                    E_ += E_new - E_old;
                    M_ += a - old;
                    data_.at("acc") << 1;
                }
                else
                    a = old;
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
                    pre_exp_[i] = std::min(1.0, std::exp(eunit_ * (2.0*i - n_neighbour) / T_));
                }
            }
            void clear() {
                data_.clear();
                data_["acc"];
                data_["E"];
                data_["M"];
                
                set_T(T_);
                
                grid_.init();
                
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
            std::map<std::string, accumulator_double> data_;
            double const eunit_;
            double T_;
            double E_;
            double M_;
            
            uint32_t const N_therm_;
            uint32_t const N_update_;
            
            //------------------- technical -------------------
            std::array<double, n_neighbour + 1> pre_exp_;
            
            //------------------- rngs -------------------
            RNG<index_type> rngL1_;
            RNG<index_type> rngL2_;
            RNG<index_type> rngL3_;
            RNG<float> rngp_;
            RNG<int> rngS_;
        };
    };
}//end namespace mc_potts

#endif //__SIM_HEADER
