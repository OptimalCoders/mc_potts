// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    24.03.2014 22:54:21 CET
// File:    std_mt_rng.hpp

#ifndef __STD_MT_RNG_HEADER
#define __STD_MT_RNG_HEADER

#include <random>
#include <string>

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
        static void seed(uint32_t const & seed) {
            seed_ = seed;
            common_rng::rng.seed(seed);
        }
        static uint32_t const & seed() {
            return seed_;
        }
        static std::string name() {
            return "std_mt_rng";
        }
    private:
        static uint32_t seed_;
        distr_t distr_;
    };
    
    template<class T, bool F>
    uint32_t helper_cl<T, F>::seed_ = 0;
    
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
        static void seed(uint32_t const & seed) {
            seed_ = seed;
            common_rng::rng.seed(seed);
        }
        static uint32_t const & seed() {
            return seed_;
        }
        static std::string name() {
            return "std_mt_rng";
        }
    private:
        static uint32_t seed_;
        distr_t distr_;
    };
    
    template<class T>
    uint32_t helper_cl<T, true>::seed_ = 0;
    
    //-------------------------matching the interface-------------------//
    
    template<typename T>
    using std_mt_rng = helper_cl<T, std::numeric_limits<T>::is_integer>;
    
}//end namespace addon

#endif //__STD_MT_RNG_HEADER


