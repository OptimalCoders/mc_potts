// Author:  Dominik Gresch <greschd@ethz.ch>
// Date:    10.04.2014 01:20:18 CEST
// File:    baseline_greschd_matrix.hpp

#ifndef ___HEADER
#define ___HEADER

#include <types.hpp>

namespace mc_potts {

    struct baseline_greschd_matrix {
        
        typedef uint8_t spin_rep_t;
        typedef std::vector<std::vector<std::vector<spin_rep_t>>> matrix_t;
        
        template<   index_type L1, 
                    index_type L2, 
                    index_type L3>
        class impl {
        public:
            
            impl(): mat_(std::vector<std::vector<std::vector<spin_rep_t>>>
                        (L1, std::vector<std::vector<spin_rep_t>>
                        (L2, std::vector<spin_rep_t>(L3)))) {};
            
            void set(   index_type const & l1,
                        index_type const & l2,
                        index_type const & l3, 
                        spin_ret_type const & s) {
                mat_.at(l1).at(l2).at(l3) = s;
            }
            spin_ret_type get(  index_type const & l1,
                                index_type const & l2,
                                index_type const & l3) const {
                return mat_.at(l1).at(l2).at(l3);
            }
        private:
            matrix_t mat_;
        };
        static std::string name() {
            return "baseline_greschd_matrix";
        }
    }; // struct baseline_greschd_matrix
    
}//end namespace mc_potts

#endif //___HEADER

