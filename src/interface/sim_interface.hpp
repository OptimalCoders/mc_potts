// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    24.03.2014 15:19:21 CET
// File:    sim.hpp

#ifndef __SIM_INTERFACE_HEADER
#define __SIM_INTERFACE_HEADER

#include <types.hpp>
#include <string>
/*
init()

set random state with dedicated (0, S) rng local.

update()

L1 * L2 * L3 attempted single spin updates

set_T()

clear "accumulator"
precompute prob

*/

namespace interface {
    using mc_potts::index_type;
    struct sim_proto_type {
        template< index_type L1
                , index_type L2
                , index_type L3
                , template<typename> class RNG
                , typename GRID
                , typename MATRIX
                >
        class impl {
            using spin_ret_type = mc_potts::spin_ret_type;
            using index_type = mc_potts::index_type;
        public:
            impl(double const & T_init, uint32_t const & N_therm, uint32_t const & N_update) {
            }
            void thermalize() {
            }
            void update() { //ensure decorr
            }
            void measure() { //feeding
            }
            mc_potts::result_struct energy() const {
                return mc_potts::result_struct(1, 1, 1, 1);
            }
            mc_potts::result_struct magn() const {
                return mc_potts::result_struct(1, 1, 1, 1);
            }
            void set_T(double const & T) {
            }
            void clear() {
            }
            spin_ret_type get(index_type const & l1
                            , index_type const & l2
                            , index_type const & l3) const {
                return 42;
            }
            static std::string spec() {
                return name() + ", " + GRID::name() + ", " + MATRIX::name() + ", " + RNG<int>::name();
            }
        };
        static std::string name() {
            return "sim_proto_type";
        }
    };
}//end namespace interface

#endif //__SIM_INTERFACE_HEADER
