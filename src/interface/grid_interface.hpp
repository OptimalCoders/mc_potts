// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    07.04.2014 20:31:03 CEST
// File:    grid_interface.hpp

#ifndef __GRID_INTERFACE_HEADER
#define __GRID_INTERFACE_HEADER

#include <types.hpp>
#include <string>

namespace interface {
    using mc_potts::index_type;
    struct grid_proto_type {
        template<index_type L1, index_type L2, index_type L3, typename MATRIX>
        class impl {
            using spin_ret_type = mc_potts::spin_ret_type;
            using index_type = mc_potts::index_type;
        public:
            void set( index_type const & l1
                    , index_type const & l2
                    , index_type const & l3
                    , spin_ret_type const & s) {
                s_ = s;
            }
            spin_ret_type get(index_type const & l1
                            , index_type const & l2
                            , index_type const & l3) const {
                return s_;
            }
            spin_ret_type get_nn(index_type const & l1
                               , index_type const & l2
                               , index_type const & l3) const {
                return 42;
            }
        private:
            spin_ret_type s_;
        };
        static std::string name() {
            return "grid_proto_type";
        }
    };
    
}//end namespace interface

#endif //__GRID_INTERFACE_HEADER
