// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    21.03.2014 09:55:10 CET
// File:    sim.hpp

#ifndef __SIM_HEADER
#define __SIM_HEADER

#include <map>

#include "grid.hpp"
#include "accum_double.hpp"
#include "random2_msk.hpp"
#include "color.hpp"

namespace potts {
    template<index_type Ht, index_type Lt>
    class sim_class {
    public:
        sim_class(): rngY_(0, Ht)
                   , rngX_(0, Lt)
                   , rngp_()
                   , rngS_(0, 2)
                   , eunit_(1)
                   , T_(10)
        {
            init();
        }
        void step() {
            int const i = rngY_();
            int const j = rngX_();
            int const shift = rngS_();
            
            auto & a = grid_.ref(i, j);
            auto const old = a;
            auto const E_old = grid_.neighbour_diff(i, j);
            
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
                    
            auto const E_new = grid_.neighbour_diff(i, j);
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
        void update() {
            for(index_type i = 0; i < Ht * Lt; ++i) {
                step();
            }
        }
        void measure() {
            assert(E_ == grid_.energy());
            assert(M_ == grid_.magn());
            
            data_.at("E") << E_ / (Ht * Lt);
            data_.at("M") << M_ / (Ht * Lt);
        }
        void init() {
            data_.clear();
            data_["acc"];
            data_["E"];
            data_["M"];
            
            set_T(T_);
            
            grid_.init();
            
            E_ = grid_.energy();
            M_ = grid_.magn();
        }
        void set_T(double const & T) {
            T_ = T;
            for(int i = 0; i < pre_exp_.size(); ++i) {
                pre_exp_[i] = std::min(1.0, std::exp(eunit_ * (2.0*i - n_neighbour) / T_));
            }
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
        typename grid_class<Ht, Lt>::grid_type const & grid() {
            return grid_.grid();
        }
    private:
        //------------------- grid -------------------
        grid_class<Ht, Lt> grid_;
        
        //------------------- measurements/physics -------------------
        std::map<std::string, accumulator_double> data_;
        double const eunit_;
        double T_;
        double E_;
        double M_;
        
        //------------------- technical -------------------
        std::array<double, n_neighbour + 1> pre_exp_;
        
        //------------------- rngs -------------------
        addon::random_class<index_type, addon::mersenne> rngY_;
        addon::random_class<index_type, addon::mersenne> rngX_;
        addon::random_class<float, addon::mersenne> rngp_;
        addon::random_class<int, addon::mersenne> rngS_;
    };
    
}//end namespace potts

#endif //__SIM_HEADER
