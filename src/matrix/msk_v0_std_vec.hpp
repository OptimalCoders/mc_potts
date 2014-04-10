// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    10.04.2014 12:20:28 CEST
// File:    msk_v0_std_vec.hpp

#ifndef __MSK_V0_STD_VEC_HEADER
#define __MSK_V0_STD_VEC_HEADER

#include <types.hpp>
#include <vector>

namespace mc_potts {
    using mc_potts::index_type;
    struct msk_v0_std_vec {
        template<index_type L1, index_type L2, index_type L3>
        class impl {
            //------------------- local typedefs -------------------
            using spin_ret_type = mc_potts::spin_ret_type;
            using state_type = mc_potts::spin_ret_type;
        public:
            //------------------- public typedefs -------------------
            using mat_type = std::vector<state_type>;
            //------------------- ctor -------------------
            impl(): mat_(L3 * L2 * L1, 0) {
            }
            //------------------- access -------------------
            inline void set(index_type const & i, index_type const & j, index_type const & k, spin_ret_type const & s) {
                mat_[L2 * L3 * i + L3 * j + k] = s;
            }
            //------------------- access -------------------
            inline spin_ret_type const & get(index_type const & i, index_type const & j, index_type const & k) const {
                return mat_[L2 * L3 * i + L3 * j + k];
            }
        private:
            //------------------- grid -------------------
            mat_type mat_;
        };
        static std::string name() {
            return "msk_v0_std_vec";
        }
    };
}//end namespace mc_potts

#endif //__MSK_V0_STD_VEC_HEADER
