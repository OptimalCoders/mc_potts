// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    26.03.2014 22:41:03 CET
// File:    lag_fib_rng.hpp

#ifndef __LAG_FIB_RNG_HEADER
#define __LAG_FIB_RNG_HEADER

#include <boost/random.hpp>
#include <string>
#include <limits>
#include <type_traits>

namespace addon {
    namespace detail {
        boost::lagged_fibonacci44497 lag_fib_engine;
    }//end namespace detail
    
    template<typename T>
    class lag_fib_rng {
    public:
        lag_fib_rng(): scale(2), shift(std::numeric_limits<T>::is_integer) {
        }
        lag_fib_rng(T const & end): scale(end), offset(0), shift(1) {
        }
        lag_fib_rng(T const & start, T const & end): scale(end-start), offset(start), shift(2) {
        }
        T operator()() {
            if (shift == 0) {
                return detail::lag_fib_engine();
            }
            if (shift == 1) {
                return scale * detail::lag_fib_engine();
            }
            return offset + scale * detail::lag_fib_engine();
        }
        static void seed(uint32_t const & sd) {
            detail::lag_fib_engine.seed(sd);
            seed_ = sd;
        }
        static uint32_t seed() {
            return seed_;
        }
        static std::string name() {
            return "lag_fib_rng";
        }
    private:
        T scale;
        T offset;
        const uint8_t shift;  //< shows, what operations are needed (for speedup. Tested!)
        static uint32_t seed_;
    };
    template<typename T>
    uint32_t lag_fib_rng<T>::seed_ = 0;
}//end namespace addon

#endif //__LAG_FIB_RNG_HEADER
