// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    09.04.2014 17:04:46 CEST
// File:    msk_v1_pbc.hpp

#ifndef __MSK_V1_PBC_HEADER
#define __MSK_V1_PBC_HEADER

#include <global.hpp>
#include <types.hpp>

#include <iostream>
#include <assert.h>
#include <array>

namespace mc_potts {
    using mc_potts::index_type;
    struct msk_v1_pbc {
        template<index_type L1, index_type L2, index_type L3, typename MATRIX>
        class impl {
            //------------------- local typedefs -------------------
            using spin_ret_type = mc_potts::spin_ret_type;
        public:
            //------------------- ctor -------------------
            impl() {
                init();
                assert(spin_ret_type(S * n_neighbour) == double(S) * n_neighbour);
            }
            void init() {
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
            inline void set(index_type const & i, index_type const & j, index_type const & k, spin_ret_type const & s) {
                return mat_.set(i, j, k, s);
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
            inline spin_ret_type get_nn(index_type const & i, index_type const & j, index_type const & k) const {
                return get(L1_prev_[i], j, k)
                     + get(L1_next_[i], j, k)
                     + get(i, L2_prev_[j], k)
                     + get(i, L2_next_[j], k)
                     + get(i, j, L2_prev_[k])
                     + get(i, j, L2_next_[k])
                     ;
            }
            //------------------- access -------------------
            inline spin_ret_type const & get(index_type const & i, index_type const & j, index_type const & k) const {
                return mat_.get(i, j, k);
            }
        private:
            //------------------- grid -------------------
            typename MATRIX::template impl<L1, L2, L3> mat_;
            
            //------------------- boundary conditions -------------------
            std::array<index_type, L1> L1_next_;
            std::array<index_type, L1> L1_prev_;
            std::array<index_type, L2> L2_next_;
            std::array<index_type, L2> L2_prev_;
            std::array<index_type, L3> L3_next_;
            std::array<index_type, L3> L3_prev_;
        };
        static std::string name() {
            return "msk_v1_pbc";
        }
    };
}//end namespace mc_potts

#endif //__MSK_V1_PBC_HEADER
