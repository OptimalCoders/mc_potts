// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    21.03.2014 09:55:15 CET
// File:    grid.hpp

#ifndef __GRID_HEADER
#define __GRID_HEADER

#include <baseline_impl_mskoenz/global.hpp>

#include <iostream>
#include <assert.h>
#include <array>

namespace mc_potts {
    template<index_type L1, index_type L2, index_type L3>
    class grid_class {
        //------------------- local typedefs -------------------
        template<typename T, int N>
        using array_type = std::array<T, N>;
    public:
        //------------------- public typedefs -------------------
        using grid_type = array_type<array_type<array_type<state_type, L3>, L2>, L1>;
        //------------------- ctor -------------------
        grid_class() {
            init();
            
        }
        void init() {
            //------------------- reset grid -------------------
            for(auto & arr: grid_)
                for(auto & ar : arr)
                    for(auto & a : ar)
                        a = (max_state - 1) / 2;
            //------------------- init boundary conditions -------------------
            for(index_type i = 0; i < L1; ++i) {
                L1_next_[i] = (i + 1) % L1;
                L1_prev_[i] = (i + L1 - 1) % L1;
            }
            for(index_type j = 0; j < L2; ++j) {
                L2_next_[j] = (j + 1) % L2;
                L2_prev_[j] = (j + L2 - 1) % L2;
            }
            for(index_type j = 0; j < L3; ++j) {
                L3_next_[j] = (j + 1) % L3;
                L3_prev_[j] = (j + L3 - 1) % L3;
            }
        }
        //------------------- access -------------------
        inline state_type & ref(index_type const & i, index_type const & j, index_type const & k) {
            //------------------- bound assert -------------------
            assert(i >= 0);
            assert(i < L1);
            assert(j >= 0);
            assert(j < L2);
            assert(k >= 0);
            assert(k < L3);
            
            return grid_[i][j][k];
        }
    //  +---------------------------------------------------+
    //  |                   const methods                   |
    //  +---------------------------------------------------+
        //------------------- print -------------------
        void print() const {
            for(auto & arr: grid_) {
                for(auto & ar: arr) {
                    for(auto & a: ar)
                        std::cout << (int)a << " ";
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
        //------------------- stencil computation -------------------
        inline state_type neighbour_diff(index_type const & i, index_type const & j, index_type const & k) const {
            auto p = grid_[i][j][k];
            return std::abs(p - grid_[L1_prev_[i]][j][k])
                 + std::abs(p - grid_[L1_next_[i]][j][k])
                 + std::abs(p - grid_[i][L2_prev_[j]][k])
                 + std::abs(p - grid_[i][L2_next_[j]][k])
                 + std::abs(p - grid_[i][j][L2_prev_[k]])
                 + std::abs(p - grid_[i][j][L2_next_[k]])
                 ;
        }
        double energy() const {
            double E = 0;
            for(index_type i = 0; i < L1; ++i) {
                for(index_type j = 0; j < L2; ++j) {
                    for(index_type k = 0; k < L3; ++k)
                        E += neighbour_diff(i, j, k);
                }
            }
            return E / 2;
        }
        double magn() const {
            double M = 0;
            for(auto & arr: grid_)
                for(auto & ar: arr)
                    for(auto & a: ar)
                        M += a;
            return M;
        }
        grid_type const & grid() const {
            return grid_;
        }
    private:
    //  +---------------------------------------------------+
    //  |                   class members                   |
    //  +---------------------------------------------------+
        //------------------- grid -------------------
        grid_type grid_;
        
        //------------------- boundary conditions -------------------
        array_type<index_type, L1> L1_next_;
        array_type<index_type, L1> L1_prev_;
        array_type<index_type, L2> L2_next_;
        array_type<index_type, L2> L2_prev_;
        array_type<index_type, L3> L3_next_;
        array_type<index_type, L3> L3_prev_;
    };
}//end namespace mc_potts

#endif //__GRID_HEADER
