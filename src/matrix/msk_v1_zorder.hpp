// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    09.04.2014 17:04:32 CEST
// File:    msk_v1_zorder.hpp

#ifndef __MSK_V1_ZORDER_HEADER
#define __MSK_V1_ZORDER_HEADER

#include <types.hpp>
#include <vector>

namespace mc_potts {
    using mc_potts::index_type;
    struct msk_v1_zorder {
        template<index_type L1, index_type L2, index_type L3>
        class impl {
            //------------------- local typedefs -------------------
            using spin_ret_type = mc_potts::spin_ret_type;
            using state_type = mc_potts::spin_ret_type;
        public:
            //------------------- public typedefs -------------------
            using mat_type = std::vector<state_type>;
            //------------------- ctor -------------------
            impl(): mat_(L3 * L2 * L1) {
                init();
                assert(state_type(S * n_neighbour) == double(S) * n_neighbour);
            }
            void init() {
                //------------------- reset grid -------------------
                for(index_type i = 0; i < L1; ++i) {
                    for(index_type j = 0; j < L2; ++j) {
                        for(index_type k = 0; k < L3; ++k)
                            get(i, j, k) = 0;
                    }
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
            inline spin_ret_type & get(index_type const & i, index_type const & j, index_type const & k) {
                return mat_[calc_z_ord(i, j, k)];
            }
            //------------------- access -------------------
            inline spin_ret_type const & get(index_type const & i, index_type const & j, index_type const & k) const {
                return mat_[calc_z_ord(i, j, k)];
            }
        private:
            //------------------- grid -------------------
            mat_type mat_;
        };
    };
}//end namespace mc_potts

#endif //__MSK_V1_ZORDER_HEADER