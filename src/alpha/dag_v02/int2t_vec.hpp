// Author:  Dominik Gresch <greschd@ethz.ch>
// Date:    26.03.2014 23:11:26 CET
// File:    int2t_vec.hpp

#ifndef __INT2T_V02
#define __INT2T_V02

#include <bitset>
#include <cassert>

namespace mc_potts {
    namespace dag_v02 {
    
        template<int N>
        class int2t_vec {
        public:
            int2t_vec(): vec_() {};
            
            void set(int const & i, uint8_t const & x) {
                assert(i < N && i >=0);
                vec_.set(2 * i, (int(x) & 2) != 0);
                vec_.set(2 * i + 1, (int(x) & 1) != 0);
            }
            
            int get(int const & i) const{
                assert(i < N && i >=0);
                return ( (vec_[2 * i] << 1) + vec_[2 * i + 1]);
            }
        private:
            std::bitset<2*N> vec_;
        };

        template<int L1, int L2, int L3>
        class int2t_3Dvec {
        public:
            int2t_3Dvec(): vec_() {};
            
            void set(int const & i, int const & j, int const & k, int const & x) {
                vec_.set(calc_z_ord_(i, j, k), x);
            }
            
            uint8_t get(int const & i, int const & j, int const & k) const{
                return vec_.get(calc_z_ord_(i, j, k));
            }
            
        private:

        //------------------- z order -------------------
            uint32_t calc_z_ord_(uint8_t const & i, uint8_t const & j, uint8_t const & k) const {
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
            
            int2t_vec<L1 * L2 * L3> vec_;
        };

    } // namespace dag_v02

} // namespace mc_potts

#endif //__INT2T_VEC_V02
