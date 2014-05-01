// Author:  Dominik Gresch <greschd@ethz.ch>
// Date:    30.04.2014 23:29:05 CEST
// File:    mkl_mt_rng.hpp

#ifndef __MKL_MT_RNG_HEADER
#define __MKL_MT_RNG_HEADER

#include <conf.hpp>
#include MKL_VSL_PATH

#include <cmath>
#include <limits>
#include <string>
#include <cstdint>
#include <type_traits>

namespace addon {
    namespace detail_mkl_mt {
        class mkl_mt_engine_class {
            uint16_t const N;
        public:
            mkl_mt_engine_class(): N(624) {
                vslNewStream(&stream_, VSL_BRNG_SFMT19937, 5489UL);
                init();
            }
            
            ~mkl_mt_engine_class() {
                vslDeleteStream(&stream_);
            }
            
            uint32_t operator()() {
                if(index_ >= N)
                    refill();
                return buffer_[index_++];
            }
            
            void refill() {
                viRngUniform( VSL_RNG_METHOD_UNIFORMBITS_STD, stream_, N, buffer_, -2147483648, 2147483647);
                index_ = 0;
            }
            
            void seed(uint32_t const & seed) {
                seed_ = seed;
                vslDeleteStream( &stream_ );
                init();
            }
            
            void init() {
                vslNewStream(&stream_, VSL_BRNG_SFMT19937, seed_);
                refill();
            }
            
        private:
            uint32_t seed_;
            int buffer_[624];
            uint16_t index_;
            VSLStreamStatePtr stream_;
        } mkl_mt_engine;
    
        template<typename T>
        class mkl_d_mt_rng {
        public:
            mkl_d_mt_rng(): scale(2), shift(std::numeric_limits<T>::is_integer) {
            }
            mkl_d_mt_rng(T const & end): scale(end), offset(0), shift(1) {
            }
            mkl_d_mt_rng(T const & start, T const & end): scale(end-start), offset(start), shift(2) {
            }
            T operator()() {
                if (shift == 0) {
                    return (mkl_mt_engine() / double(1ul << 32));
                }
                if (shift == 1) {
                    return scale * (mkl_mt_engine() / double(1ul << 32));
                }
                return offset + scale * (mkl_mt_engine() / double(1ul << 32));
            }
            static void seed(uint32_t const & sd) {
                mkl_mt_engine.seed(sd);
                seed_ = sd;
            }
            static uint32_t seed() {
                return seed_;
            }
            static std::string name() {
                return "mkl_mt_rng";
            }
        private:
            T scale;
            T offset;
            const uint8_t shift;  //< shows, what operations are needed (for speedup. Tested!)
            static uint32_t seed_;
        };
        
        template<typename T>
        uint32_t mkl_d_mt_rng<T>::seed_ = 0;
        
        template<typename T>
        class eff_int_mt_rng {
        public:
            eff_int_mt_rng(): bit_need_(1), idx_(0), scale(2), shift(1) {
            }
            eff_int_mt_rng(T const & end): bit_need_(std::ceil(std::log2(end))), idx_(0), scale(end), offset(0), shift(1) {
            }
            eff_int_mt_rng(T const & start, T const & end): bit_need_(std::ceil(std::log2(end-start))), idx_(0), scale(end-start), offset(start), shift(2) {
            }
            T operator()() {
                if(idx_ == 0) {
                    store_ = (uint64_t(mkl_mt_engine()) << 32) + mkl_mt_engine();
                    idx_ = 64 / bit_need_;
                    //~ store_ = mkl_mt_engine();
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
                mkl_mt_engine.seed(sd);
                seed_ = sd;
            }
            static uint32_t seed() {
                return seed_;
            }
            static std::string name() {
                return "eff_mt_rng";
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
        uint32_t eff_int_mt_rng<T>::seed_ = 0;
        
        template<typename T, bool b>
        struct impl_chooser {
            template<typename U>
            using type = mkl_d_mt_rng<U>;
        };
        
        template<typename T>
        struct impl_chooser<T, true> {
            template<typename U>
            using type = eff_int_mt_rng<U>;
        };
    } // namespace detail_mkl_mt
    

    template<typename T>
    using mkl_mt_rng = typename detail_mkl_mt::impl_chooser<T, std::numeric_limits<T>::is_integer>::template type<T>;
    
} // namespace addon

#endif //__MKL_MT_RNG_HEADER




