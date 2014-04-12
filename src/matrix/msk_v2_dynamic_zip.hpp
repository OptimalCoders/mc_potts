// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    12.04.2014 13:08:40 CEST
// File:    msk_v2_dynamic_zip.hpp

#ifndef __MSK_V2_DYNAMIC_ZIP_HEADER
#define __MSK_V2_DYNAMIC_ZIP_HEADER

#include <types.hpp>
#include <bitset>
namespace mc_potts {
    using mc_potts::index_type;
    struct msk_v2_dynamic_zip {
        template<index_type L1, index_type L2, index_type L3>
        class impl {
            //------------------- local typedefs -------------------
            using spin_ret_type = mc_potts::spin_ret_type;
            using state_type = uint8_t;
            uint8_t const mask[4];
            uint8_t const shift[4];
        public:
            //------------------- ctor -------------------
            impl(): mask{0x03, 0x0C, 0x30, 0xC0}, shift{0, 2, 4, 6}, mat_(new state_type[L3 * L2 * L1 / 4 + 1]) {
            }
            ~impl() {
                delete[] mat_;
            }
            //------------------- access -------------------
            inline void set(index_type const & i, index_type const & j, index_type const & k, spin_ret_type const & s) {
                index_type pos = i*L2*L3 + j*L3 + k;
                uint8_t loc = pos & 3;
                uint8_t & byte = mat_[pos >> 2];
                byte &= (~mask[loc]);
                byte |= (s << shift[loc]);
            }
            //------------------- access -------------------
            inline spin_ret_type get(index_type const & i, index_type const & j, index_type const & k) const {
                index_type pos = i*L2*L3 + j*L3 + k;
                uint8_t loc = pos & 3;
                return (mat_[pos >> 2] >> shift[loc]) & 3;
            }
        private:
            //------------------- grid -------------------
            state_type * mat_;
        };
        static std::string name() {
            return "msk_v2_dynamic_zip";
        }
    };
}//end namespace mc_potts

#endif //__MSK_V2_DYNAMIC_ZIP_HEADER
