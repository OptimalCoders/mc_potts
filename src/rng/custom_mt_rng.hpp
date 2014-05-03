// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    10.04.2014 19:08:00 CEST
// File:    custom_mt_rng.hpp

#ifndef __CUSTOM_MT_RNG_HEADER
#define __CUSTOM_MT_RNG_HEADER

#include <limits>
#include <string>
#include <cstdint>
#include <type_traits>

namespace addon {
    namespace detail {
        class custom_mt_engine_class {
            uint16_t const N;
            uint16_t const M;
            uint32_t const HI;
            uint32_t const LO;
        public:
            custom_mt_engine_class(): N(624), M(397), HI(0x80000000), LO(0x7fffffff), A{0, 0x9908b0df} {
                seed(5489UL);
            }
            uint32_t operator()() {
                uint32_t e;
                if(index_ >= N)
                    refill();
                
                e = y[index_++];
                /* Tempering */
                e ^= (e >> 11);
                e ^= (e <<  7) & 0x9d2c5680;
                e ^= (e << 15) & 0xefc60000;
                e ^= (e >> 18);
         
                return e;
            }
            void refill() {
                int i;
                uint32_t h;
                
                for(i = 0; i < N - M; ++i) {
                    h = (y[i] & HI) | (y[i+1] & LO);
                    y[i] = y[i+M] ^ (h >> 1) ^ A[h & 1];
                }
                for (; i<N-1; ++i) {
                    h = (y[i] & HI) | (y[i+1] & LO);
                    y[i] = y[i+(M-N)] ^ (h >> 1) ^ A[h & 1];
                }

                h = (y[N-1] & HI) | (y[0] & LO);
                y[N-1] = y[M-1] ^ (h >> 1) ^ A[h & 1];
                index_ = 0;
            }
            void init() {
                y[0] = seed_;
                for (uint16_t i = 1; i < N; ++i) {
                    y[i] = (1812433253UL * (y[i-1] ^ (y[i-1] >> 30)) + i);
                }
                refill();
            }
            void seed(uint32_t const & seed) {
                seed_ = seed;
                init();
            }
        private:
            uint32_t seed_;
            const uint32_t A[2];
            uint32_t y[624];
            uint16_t index_;
        } custom_mt_engine;
    
        template<typename T>
        class custom_d_mt_rng {
        public:
            custom_d_mt_rng(): scale(2), shift(std::numeric_limits<T>::is_integer) {
            }
            custom_d_mt_rng(T const & end): scale(end), offset(0), shift(1) {
            }
            custom_d_mt_rng(T const & start, T const & end): scale(end-start), offset(start), shift(2) {
            }
            T operator()() {
                if (shift == 0) {
                    return (detail::custom_mt_engine() / double(1ul << 32));
                }
                if (shift == 1) {
                    return scale * (detail::custom_mt_engine() / double(1ul << 32));
                }
                return offset + scale * (detail::custom_mt_engine() / double(1ul << 32));
            }
            static void seed(uint32_t const & sd) {
                detail::custom_mt_engine.seed(sd);
                seed_ = sd;
            }
            static uint32_t seed() {
                return seed_;
            }
            static std::string name() {
                return "custom_mt_rng";
            }
        private:
            T scale;
            T offset;
            const uint8_t shift;  //< shows, what operations are needed (for speedup. Tested!)
            static uint32_t seed_;
        };
        template<typename T>
        uint32_t custom_d_mt_rng<T>::seed_ = 0;
        
        template<typename T>
        class custom_int_mt_rng {
        public:
            custom_int_mt_rng(): bit_need_(1), idx_(0), scale(2), shift(1) {
            }
            custom_int_mt_rng(T const & end): bit_need_(std::ceil(std::log2(end))), idx_(0), scale(end), offset(0), shift(1) {
            }
            custom_int_mt_rng(T const & start, T const & end): bit_need_(std::ceil(std::log2(end-start))), idx_(0), scale(end-start), offset(start), shift(2) {
            }
            T operator()() {
                if(idx_ == 0) {
                    store_ = (uint64_t(detail::custom_mt_engine()) << 32) + detail::custom_mt_engine();
                    idx_ = 64 / bit_need_;
                    //~ store_ = detail::custom_mt_engine();
                    //~ idx_ = 32 / bit_need_;
                }
                double res = ((store_ & ((1 << bit_need_) - 1)) / double(1ul << bit_need_));
                store_ >>= bit_need_;
                --idx_;
                if (shift == 1) {
                    return scale * res;
                }
                return offset + scale * res;
            }
            static void seed(uint32_t const & sd) {
                detail::custom_mt_engine.seed(sd);
                seed_ = sd;
            }
            static uint32_t seed() {
                return seed_;
            }
            static std::string name() {
                return "custom_mt_rng";
            }
        private:
            uint8_t const bit_need_;
            uint8_t idx_;
            uint64_t store_;
            //~ uint32_t store_;
            T scale;
            T offset;
            const uint8_t shift;  //< shows, what operations are needed (for speedup. Tested!)
            static uint32_t seed_;
        };
        template<typename T>
        uint32_t custom_int_mt_rng<T>::seed_ = 0;
        
        template<typename T, bool b>
        struct custom_mt_impl_chooser {
            template<typename U>
            using type = custom_d_mt_rng<U>;
        };
        template<typename T>
        struct custom_mt_impl_chooser<T, true> {
            template<typename U>
            using type = custom_int_mt_rng<U>;
        };
    }//end namespace detail
    
    template<typename T>
    using custom_mt_rng = typename detail::custom_mt_impl_chooser<T, std::numeric_limits<T>::is_integer>::template type<T>;
    
}//end namespace addon


#endif //__CUSTOM_MT_RNG_HEADER

