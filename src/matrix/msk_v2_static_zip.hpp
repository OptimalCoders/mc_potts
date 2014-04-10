// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    10.04.2014 17:37:44 CEST
// File:    msk_v2_static_zip.hpp

#ifndef __MSK_V2_STATIC_ZIP_HEADER
#define __MSK_V2_STATIC_ZIP_HEADER

#include <types.hpp>

namespace mc_potts {
    using mc_potts::index_type;
    struct msk_v2_static_zip {
        template<index_type L1, index_type L2, index_type L3>
        class impl {
            //------------------- local typedefs -------------------
            using spin_ret_type = mc_potts::spin_ret_type;
            using state_type = mc_potts::spin_ret_type;
        public:
            //------------------- ctor -------------------
            impl() {
            }
            //------------------- access -------------------
            inline void set(index_type const & i, index_type const & j, index_type const & k, spin_ret_type const & s) {
                mat_[i][j][k] = s;
            }
            //------------------- access -------------------
            inline spin_ret_type get(index_type const & i, index_type const & j, index_type const & k) const {
                return mat_[i][j][k];
            }
        private:
            //------------------- grid -------------------
            state_type mat_[L1][L2][L3];
        };
        static std::string name() {
            return "msk_v2_static_zip";
        }
    };
}//end namespace mc_potts

#endif //__MSK_V2_STATIC_ZIP_HEADER
