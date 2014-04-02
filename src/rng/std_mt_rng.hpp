// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    24.03.2014 22:54:21 CET
// File:    std_mt_rng.hpp

#ifndef __STD_MT_RNG_HEADER
#define __STD_MT_RNG_HEADER

#include <random>

namespace addon {
    
    struct common_rng {
        typedef std::mt19937 rng_t;
        static rng_t rng;
    };
    
    common_rng::rng_t common_rng::rng(42);
    
    
    //------------------------------------------------------------------//
    //            general template for floating point types             //
    //------------------------------------------------------------------//
    
    template<class T, bool F>
    class helper_cl {
    //-------------------------local typedefs---------------------------//
        typedef std::uniform_real_distribution<T> distr_t;

    public:
        helper_cl(T const & lower = T(0), T const & upper = T(1)): distr_(lower, upper)  {
        }
        T operator()() {
            return distr_(common_rng::rng);
        }
        void set_seed(uint32_t const & seed) {
            seed_ = seed;
            common_rng::rng.seed(seed);
        }
        uint32_t const & seed() const {
            return seed_;
        }
    private:
        uint32_t seed_;
        distr_t distr_;
    };
    
    //------------------------------------------------------------------//
    //                      specialization for int types                //
    //------------------------------------------------------------------//
    
    template<class T>
    class helper_cl<T, true> {
    //-------------------------local typedefs---------------------------//
        typedef std::uniform_int_distribution<int> distr_t;

    public:
        helper_cl(T const & lower = T(0), T const & upper = T(2)): distr_(lower, upper - 1)  {
        }
        T operator()() {
            return distr_(common_rng::rng);
        }
        void set_seed(uint32_t const & seed) {
            seed_ = seed;
            common_rng::rng.seed(seed);
        }
        uint32_t const & seed() const {
            return seed_;
        }
    private:
        uint32_t seed_;
        distr_t distr_;
    };
    
    //-------------------------matching the interface-------------------//
    
    template<typename T>
    using std_mt_rng = helper_cl<T, std::numeric_limits<T>::is_integer>;
    
}//end namespace addon

#endif //__STD_MT_RNG_HEADER


