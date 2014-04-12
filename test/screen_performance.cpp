// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    11.04.2014 14:32:54 CEST
// File:    screen_performance.cpp

#include <beta.hpp>
#include <meta_tpl.hpp>
#include <addon/performance.hpp>

#include <iostream>
#include <sstream>

using namespace mc_potts;

struct std_mt_wrap {
    template<typename T>
    using type = addon::std_mt_rng<T>;
};
struct cst_mt_wrap {
    template<typename T>
    using type = addon::custom_mt_rng<T>;
};
struct lag_fib_wrap {
    template<typename T>
    using type = addon::lag_fib_rng<T>;
};

using sim =    list<msk_v1_sim, baseline_greschd_sim>::type;
using grid =   list<msk_v1_pbc, baseline_greschd_grid>::type;
using matrix = list<msk_v1_zorder, msk_v0_c_array_dynamic, msk_v0_std_vec, msk_v2_dynamic_zip>::type;
using rng =    list<cst_mt_wrap, lag_fib_wrap>::type;
using Lsize =  list<int_type<32>, int_type<64>, int_type<128>, int_type<256>, int_type<512>, int_type<1024>>::type;
using Temp =   list<rational_type<1, 1>>::type;

using all = typename permut_list<sim, grid, matrix, rng, Lsize, Temp>::type;

template<typename T, typename L, template<typename> class U>
struct foreach_impl {
    static void apply() {
        U<T>::apply();
        foreach_impl<typename L::type, typename L::next, U>::apply();
    }
};
template<typename T, template<typename> class U>
struct foreach_impl<T, node_end, U> {
    static void apply() {
        U<T>::apply();
    }
};

template<typename L, template<typename> class U>
struct foreach {
    static void apply() {
        foreach_impl<typename L::type, typename L::next, U>::apply();
    }
};

//------------------- ugly globals (q&d) -------------------
int idx = 0;
int max = size<all>::value;

template<typename T>
struct measure {
    using rng_wrap = typename get<T, 3>::type;
    template<typename U>
    using rngt = typename rng_wrap::template type<U>;
    
    static void apply() {
        using sim  = typename get<T, 0>::type;
        using grid = typename get<T, 1>::type;
        using mat  = typename get<T, 2>::type;
        using Lt = typename get<T, 4>::type;
        //~ using Tt = typename get<T, 5>::type;
        
        const int N = 100;
        const int L = Lt::value;
        //~ const double Temp = Tt::get();
        
        constexpr std::array<double, 1> T_arr{.1};
        
        std::cout << typeid(T).name() << std::endl;
        std::cout << idx++ << "/" << max << std::endl;
        
        for(auto & Temp : T_arr) {
            try {
                typename sim::template impl<L, L, L, rngt, grid, mat> s(Temp, 10, N);
                std::stringstream ss;
                ss << s.spec() << ", " << L << ", " << Temp;
                MEASURE_DIV(s.update(), ss.str(), N)
            }
            catch(...) {
                std::cout << " fail" << std::endl;
            }
        }
    }
};


int main(int argc, char* argv[]) {
    
    foreach<all, measure>::apply();
    
    P_RESULTS()
    P_SPEEDUP()
    
    return 0;
}
