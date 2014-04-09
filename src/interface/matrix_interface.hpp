// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    07.04.2014 20:27:09 CEST
// File:    matrix_interface.hpp

#ifndef __MATRIX_INTERFACE_HEADER
#define __MATRIX_INTERFACE_HEADER

#include <types.hpp>

namespace interface {
    using mc_potts::index_type;
    struct matrix_proto_type {
        template<index_type L1, index_type L2, index_type L3>
        class impl {
            using spin_ret_type = mc_potts::spin_ret_type;
            using index_type = mc_potts::index_type;
        public:
            spin_ret_type & get(  index_type const & l1
                                , index_type const & l2
                                , index_type const & l3) {
                return s;
            }
            spin_ret_type const & get(index_type const & l1
                                    , index_type const & l2
                                    , index_type const & l3) const {
                return s;
            }
        private:
            spin_ret_type s;
        };
    };
}//end namespace inferface

#endif //__MATRIX_INTERFACE_HEADER
