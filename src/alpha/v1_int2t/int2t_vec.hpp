// Author:  Dominik Gresch <greschd@ethz.ch>
// Date:    26.03.2014 23:11:26 CET
// File:    int2t_vec.hpp

#ifndef __INT2T_VEC_HEADER
#define __INT2T_VEC_HEADER

#include <bitset>
#include <cassert>

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
        vec_.set(i * L2 * L3 + j * L3 + k, x);
    }
    
    uint8_t get(int const & i, int const & j, int const & k) const{
        return vec_.get(i * L2 * L3 + j * L3 + k);
    }
    
private:
    int2t_vec<L1 * L2 * L3> vec_;
};


#endif //__INT2T_VEC_HEADER
