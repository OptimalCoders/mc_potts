// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    23.05.2014 10:33:00 CEST
// File:    msk_v3_zip_order.hpp

#ifndef __MSK_V3_ZIP_ORDER_HEADER
#define __MSK_V3_ZIP_ORDER_HEADER

#include <types.hpp>
#include <vector>

namespace mc_potts {
    using mc_potts::index_type;
    struct msk_v3_zip_order {
        template<index_type L1, index_type L2, index_type L3>
        class impl {
            //------------------- local typedefs -------------------
            using spin_ret_type = mc_potts::spin_ret_type;
            using state_type = mc_potts::spin_ret_type;
            uint8_t const mask[4];
            uint8_t const shift[4];
        public:
            //------------------- public typedefs -------------------
            using mat_type = std::vector<state_type>;
            //------------------- ctor -------------------
            impl(): mask{0x03, 0x0C, 0x30, 0xC0}, shift{0, 2, 4, 6}, mat_(L3 * L2 * L1 / 4 + 1, 0) {
                int L3_ = std::pow(2, std::ceil(std::log2(L3)));
                int L2_ = std::pow(2, std::ceil(std::log2(L2)));
                int L1_ = std::pow(2, std::ceil(std::log2(L1)));
                mat_.resize(L3_ * L2_ * L1_ / 4 + 1);
            }
            //------------------- z order -------------------
            uint64_t calc_z_ord(uint16_t const & i, uint16_t const & j, uint16_t const & k) const {
                
                uint64_t x = i;
                uint64_t y = j;
                uint64_t z = k;
                
                x = (x | (x << 16)) & 0x0000FF0000FF;
                y = (y | (y << 16)) & 0x0000FF0000FF;
                z = (z | (z << 16)) & 0x0000FF0000FF;
                
                x = (x | (x <<  8)) & 0x00F00F00F00F;
                y = (y | (y <<  8)) & 0x00F00F00F00F;
                z = (z | (z <<  8)) & 0x00F00F00F00F;

                x = (x | (x <<  4)) & 0x0C30C30C30C3;
                y = (y | (y <<  4)) & 0x0C30C30C30C3;
                z = (z | (z <<  4)) & 0x0C30C30C30C3;
                
                x = (x | (x <<  2)) & 0x249249249249;
                y = (y | (y <<  2)) & 0x249249249249;
                z = (z | (z <<  2)) & 0x249249249249;

                return x | (y << 1) | (z << 2);
            }
            //------------------- access -------------------
            inline void set(index_type const & i, index_type const & j, index_type const & k, spin_ret_type const & s) {
                index_type pos = calc_z_ord(i, j, k);
                uint8_t loc = pos & 3;
                uint8_t & byte = mat_[pos >> 2];
                byte &= (~mask[loc]);
                byte |= (s << shift[loc]);
            }
            //------------------- access -------------------
            inline spin_ret_type get(index_type const & i, index_type const & j, index_type const & k) const {
                index_type pos = calc_z_ord(i, j, k);
                uint8_t loc = pos & 3;
                return (mat_[pos >> 2] >> shift[loc]) & 3;
            }
            
            
        private:
            //------------------- grid -------------------
            mat_type mat_;
        };
        static std::string name() {
            return "msk_v3_zip_order";
        }
    };
}//end namespace mc_potts

#endif //__MSK_V3_ZIP_ORDER_HEADER
