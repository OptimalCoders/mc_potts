// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    26.03.2014 22:41:03 CET
// File:    lag_fib_rng.hpp

#ifndef __LAG_FIB_RNG_HEADER
#define __LAG_FIB_RNG_HEADER

#include <boost/random.hpp>
#include <type_traits>
#include <ctime>

namespace addon {
    namespace detail {
        boost::lagged_fibonacci44497 lag_fib_engine;
    }//end namespace detail
    
    template<typename T>
    class lag_fib_rng {
    public:
        lag_fib_rng(): scale(2), shift(std::numeric_limits<T>::is_integer) {
            init();
        }
        lag_fib_rng(T const & end): scale(end), offset(0), shift(1) {
            init();
        }
        lag_fib_rng(T const & start, T const & end): scale(end-start), offset(start), shift(2) {
            init();
        }
        inline T operator()() {
            if (shift == 0) {
                return detail::lag_fib_engine();
            }
            if (shift == 1) {
                return scale * detail::lag_fib_engine();
            }
            return offset + scale * detail::lag_fib_engine();
        }
        void set_seed(uint32_t const & sd) {
            detail::lag_fib_engine.seed(sd);
            seed_ = sd;
        }
        inline uint32_t seed() const {
            return seed_;
        }
    private:
        inline void init() {
            for(unsigned i = 0; i < 100; ++i) {
                //~ detail::lag_fib_engine(); //warming up the rng
                //fibbonaci has for several seconds the same first number even though the seed changes every second
            }
        }
        T scale;
        T offset;
        const uint8_t shift;  //< shows, what operations are needed (for speedup. Tested!)
        uint64_t seed_;
    };
}//end namespace addon

#endif //__LAG_FIB_RNG_HEADER
