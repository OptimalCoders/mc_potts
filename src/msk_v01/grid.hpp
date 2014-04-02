// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    21.03.2014 09:55:15 CET
// File:    grid.hpp

#ifndef ___MSK_V01_GRID_HEADER
#define ___MSK_V01_GRID_HEADER

#include <msk_v01/global.hpp>

#include <iostream>
#include <assert.h>
#include <array>

namespace mc_potts {
    namespace msk_v1 {
        template<index_type L1, index_type L2, index_type L3>
        class grid_class {
            //------------------- local typedefs -------------------
            template<typename T, int N>
            using array_type = std::array<T, N>;
        public:
            //------------------- public typedefs -------------------
            //~ using grid_type = array_type<state_type, L3 * L2 * L1>; //doesn't work for L > 203
            using grid_type = std::vector<state_type>;
            //------------------- ctor -------------------
            grid_class(): grid_(L3 * L2 * L1) {
                init();
                assert(state_type(max_state * n_neighbour) == double(max_state) * n_neighbour);
            }
            void init() {
                //------------------- reset grid -------------------
                for(index_type i = 0; i < L1; ++i) {
                    for(index_type j = 0; j < L2; ++j) {
                        for(index_type k = 0; k < L3; ++k)
                            get(i, j, k) = (max_state - 1) / 2;
                    }
                }
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
            //------------------- z order -------------------
            uint32_t calc_z_ord(uint8_t const & i, uint8_t const & j, uint8_t const & k) const {
                uint32_t x = i;
                uint32_t y = j;
                uint32_t z = k;

                x = (x | (x << 8)) & 0x0000F00F;
                y = (y | (y << 8)) & 0x0000F00F;
                z = (z | (z << 8)) & 0x0000F00F;

                x = (x | (x << 4)) & 0x000C30C3;
                y = (y | (y << 4)) & 0x000C30C3;
                z = (z | (z << 4)) & 0x000C30C3;
                
                x = (x | (x << 2)) & 0x00249249;
                y = (y | (y << 2)) & 0x00249249;
                z = (z | (z << 2)) & 0x00249249;

                return x | (y << 1) | (z << 2);
            }
            //------------------- access -------------------
            inline state_type & get(index_type const & i, index_type const & j, index_type const & k) {
                return grid_[calc_z_ord(i, j, k)];
                //~ return grid_[L2*L3*i + L3*j + k];
            }
        //  +---------------------------------------------------+
        //  |                   const methods                   |
        //  +---------------------------------------------------+
            //------------------- print -------------------
            void print() const {
                for(index_type i = 0; i < L1; ++i) {
                    for(index_type j = 0; j < L2; ++j) {
                        for(index_type k = 0; k < L3; ++k)
                            std::cout << (int)get(i, j, k) << " ";
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
            //------------------- stencil computation -------------------
            inline state_type neighbour(index_type const & i, index_type const & j, index_type const & k) const {
                return get(L1_prev_[i], j, k)
                     + get(L1_next_[i], j, k)
                     + get(i, L2_prev_[j], k)
                     + get(i, L2_next_[j], k)
                     + get(i, j, L2_prev_[k])
                     + get(i, j, L2_next_[k])
                     ;
            }
            double energy() const {
                double E = 0;
                for(index_type i = 0; i < L1; ++i) {
                    for(index_type j = 0; j < L2; ++j) {
                        for(index_type k = 0; k < L3; ++k)
                            E += (get(i, j, k) - (max_state - 1) / 2.0)
                               * (neighbour(i, j, k) - n_neighbour * (max_state - 1) / 2.0);
                    }
                }
                return E / 2;
            }
            double magn() const {
                double M = 0;
                for(index_type i = 0; i < L1; ++i)
                    for(index_type j = 0; j < L2; ++j)
                        for(index_type k = 0; k < L3; ++k)
                            M += get(i, j, k);
                return M;
            }
            //------------------- access -------------------
            inline state_type const & get(index_type const & i, index_type const & j, index_type const & k) const {
                return grid_[calc_z_ord(i, j, k)];
                //~ return grid_[L2*L3*i + L3*j + k];
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
    }//end namespace msk_v1
}//end namespace mc_potts

#endif //___MSK_V01_GRID_HEADER
