// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    25.05.2014 06:54:14 CEST
// File:    sse_z_order.cpp

#define ENABLE_MICRO
#include <addon/micro_bench.hpp>
#include <addon/performance.hpp>

#include "emmintrin.h"
#include "pmmintrin.h"

#include <iostream>
#include <bitset>
#include <sstream>

#define RED "\033[0;31m"
#define REDB "\033[1;31m"
#define GREEN "\033[0;32m"
#define GREENB "\033[1;32m"
#define YELLOW "\033[0;33m"
#define YELLOWB "\033[1;33m"
#define BLUE "\033[0;34m"
#define BLUEB "\033[1;34m"
#define LILA "\033[0;35m"
#define LILAB "\033[1;35m"
#define TUR "\033[0;36m"
#define TURB "\033[1;36m"
#define WHITE "\033[0;37m"
#define WHITEB "\033[1;37m"
#define NONE "\033[0m" 

uint ctrl1 = 1;

uint64_t calc_z_ord(uint16_t const & i, uint16_t const & j, uint16_t const & k) {
    START_MICRO("norm")
    uint64_t x = i;
    uint64_t y = j;
    //~ uint64_t z = k;
    
    x = (x | (x << 16)) & 0x0000FF0000FF;
    y = (y | (y << 16)) & 0x0000FF0000FF;
    //~ z = (z | (z << 16)) & 0x0000FF0000FF;
    
    x = (x | (x << 8)) & 0x00F00F00F00F;
    y = (y | (y << 8)) & 0x00F00F00F00F;
    //~ z = (z | (z << 8)) & 0x00F00F00F00F;

    x = (x | (x << 4)) & 0x0C30C30C30C3;
    y = (y | (y << 4)) & 0x0C30C30C30C3;
    //~ z = (z | (z << 4)) & 0x0C30C30C30C3;
    
    x = (x | (x << 2)) & 0x249249249249;
    y = (y | (y << 2)) & 0x249249249249;
    //~ z = (z | (z << 2)) & 0x249249249249;
    
    uint64_t res = x | (y << 1);
    //~ uint64_t res = x | (y << 1) | (z << 2);
    STOP_MICRO()
    
    ctrl1 += res;
    return res;
}

uint ctrl2 = 1;

uint64_t const mask16[2] = {0x0000FF0000FF, 0x0000FF0000FF};
uint64_t const mask8[2]  = {0x00F00F00F00F, 0x00F00F00F00F};
uint64_t const mask4[2]  = {0x0C30C30C30C3, 0x0C30C30C30C3};
uint64_t const mask2[2]  = {0x249249249249, 0x249249249249};

uint64_t sse_calc_z_ord(uint16_t const & i, uint16_t const & j, uint16_t const & k) {
    START_MICRO("sse")
    uint64_t xy[4] = {i, j};

    __m128i m16 = _mm_loadu_si128((__m128i*)mask16);
    __m128i m8 = _mm_loadu_si128((__m128i*)mask8);
    __m128i m4 = _mm_loadu_si128((__m128i*)mask4);
    __m128i m2 = _mm_loadu_si128((__m128i*)mask2);

    __m128i xy_a = _mm_loadu_si128((__m128i*)xy);
    //~ __m128i xy_a = _mm_set_epi32(0, i, 0, j);
    
    __m128i xy16 = _mm_slli_epi64(xy_a, 16);
    __m128i xy_b = _mm_or_si128(xy_a, xy16);
    __m128i xy_c = _mm_and_si128(xy_b, m16);
    
    __m128i xy8 = _mm_slli_epi64(xy_c, 8);
    __m128i xy_d = _mm_or_si128(xy_c, xy8);
    __m128i xy_e = _mm_and_si128(xy_d, m8);
    
    __m128i xy4 = _mm_slli_epi64(xy_e, 4);
    __m128i xy_f = _mm_or_si128(xy_e, xy4);
    __m128i xy_g = _mm_and_si128(xy_f, m4);
    
    __m128i xy2 = _mm_slli_epi64(xy_g, 2);
    __m128i xy_h = _mm_or_si128(xy_g, xy2);
    __m128i xy_i = _mm_and_si128(xy_h, m2);
    
    _mm_stream_si128((__m128i*)xy, xy_i);
    
    uint64_t res = xy[0] | (xy[1] << 1);
    STOP_MICRO()

    ctrl2 += res;
    return res;
}


int main(int argc, char* argv[]) {
    
    //------------------- demo -------------------
    //~ int32_t c[] = {59, 59999, 527};    
    int32_t c[] = {3+256, 3+256, 1};    
    std::string color[] = {REDB, GREENB, BLUEB};
    
    for(uint i = 0; i < 3; ++i) {
        for(uint j = i; j < 3; ++j) {
            std::cout << " ";
        }
        std::stringstream ss;
        ss << std::bitset<32>(c[i]);
        std::string out = ss.str();
        
        for(uint j = 11; j < 32; ++j) {
            std::cout << color[i] << out[j] << "  ";
        }
        std::cout << NONE << std::endl;
    }
    
    std::stringstream ss;
    ss << std::bitset<64>(calc_z_ord(c[0], c[1], c[2]));
    
    std::string out = ss.str();
    
    for(uint i = 0; i < out.length(); ++i) {
        std::cout << color[2-((i+2)%3)] << out[i];
    }
    std::cout << NONE << std::endl;
    
    sse_calc_z_ord(c[0], c[1], c[2]);
    
    //------------------- benchmark -------------------
    MEASURE(calc_z_ord(c[0], c[1], c[2]), "z_order normal")
    MEASURE(sse_calc_z_ord(c[0], c[1], c[2]), "z_order sse")
    
    P_RESULTS()
    P_MICRO()
    std::cout << ctrl1 << std::endl;
    std::cout << ctrl2 << std::endl;
    
    return 0;
}
