// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    21.03.2014 09:55:15 CET
// File:    grid.hpp

#ifndef __GRID_HEADER
#define __GRID_HEADER

#include <iostream>
#include <array>
#include <assert.h>

#include "global.hpp"

namespace potts {
    template<index_type Ht, index_type Lt>
    class grid_class {
        //------------------- local typedefs -------------------
        template<typename T, int N>
        using array_type = std::array<T, N>;
    public:
        //------------------- ctor -------------------
        grid_class(): H(Ht), L(Lt) {
            init();
            
        }
        void init() {
            //------------------- reset grid -------------------
            state_type p;
            
            for(auto & ar: grid_)
                for(auto & a : ar)
                    a = (max_state - 1) / 2;
            //------------------- init boundary conditions -------------------
            for(index_type i = 0; i < H; ++i) {
                y_next_[i] = (i + 1) % H;
                y_prev_[i] = (i + H - 1) % H;
            }
            for(index_type j = 0; j < L; ++j) {
                x_next_[j] = (j + 1) % L;
                x_prev_[j] = (j + L - 1) % L;
            }
        }
        //------------------- access -------------------
        inline state_type & ref(index_type const & i, index_type const & j) {
            //------------------- bound assert -------------------
            assert(i >= 0);
            assert(i < H);
            assert(j >= 0);
            assert(j < L);
            
            return grid_[i][j];
        }
    //  +---------------------------------------------------+
    //  |                   const methods                   |
    //  +---------------------------------------------------+
        //------------------- print -------------------
        void print() const {
            for(auto & ar: grid_) {
                for(auto & a: ar)
                    std::cout << (int)a << " ";
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
        //------------------- stencil computation -------------------
        inline state_type neighbour_diff(index_type const & i, index_type const & j) const {
            auto p = grid_[i][j];
            return std::abs(p - grid_[y_prev_[i]][j])
                 + std::abs(p - grid_[y_next_[i]][j])
                 + std::abs(p - grid_[i][x_prev_[j]])
                 + std::abs(p - grid_[i][x_next_[j]])
                 ;
        }
        double energy() const {
            double E = 0;
            for(index_type i = 0; i < Ht; ++i) {
                for(index_type j = 0; j < Lt; ++j) {
                    E += neighbour_diff(i, j);
                }
            }
            return E / 2;
        }
        double magn() const {
            double M = 0;
            for(auto & ar: grid_)
                for(auto & a: ar)
                    M += a;
            return M;
        }
    //  +---------------------------------------------------+
    //  |                   class members                   |
    //  +---------------------------------------------------+
        index_type const L;
        index_type const H;
    private:
        //------------------- grid -------------------
        array_type<array_type<state_type, Lt>, Ht> grid_;
        
        //------------------- boundary conditions -------------------
        array_type<index_type, Lt> x_next_;
        array_type<index_type, Lt> x_prev_;
        array_type<index_type, Ht> y_next_;
        array_type<index_type, Ht> y_prev_;
    };
    
}//end namespace potts

#endif //__GRID_HEADER
