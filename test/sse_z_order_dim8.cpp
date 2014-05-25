// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    25.05.2014 08:40:51 CEST
// File:    sse_z_order_dim8.cpp

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

uint64_t calc_z_ord(uint8_t c[8]) {
    uint64_t d[8];
    
    START_MICRO("norm")
    
    uint64_t res = 0;
    for(uint8_t i = 0; i < 8; ++i) {
        d[i] = c[i];
        d[i] = (d[i] | (d[i] << 28)); //mask not necessary
        d[i] = (d[i] | (d[i] << 14)) & 0x0003000300030003;
        d[i] = (d[i] | (d[i] << 7)) & 0x0101010101010101;
        res |=  (d[i] << i);
    }
    STOP_MICRO()
    
    ctrl1 += res;
    return res;
}
/*
00000000 00000000 00000000 00000000 00000000 00000000 00000000 abcdefgh

(d | d << 28)

00000000 00000000 00000000 00000000 00000000 00000000 00000000 abcdefgh
00000000 00000000 00000000 abcdefgh 00000000 00000000 00000000 00000000
* 
00000000 00000000 00000000 abcdefgh 00000000 00000000 00000000 abcdefgh

& 0x0000000F0000000F

00000000 00000000 00000000 0000abcd efgh0000 00000000 00000000 abcdefgh
00000000 00000000 00000000 00001111 00000000 00000000 00000000 00001111

00000000 00000000 00000000 0000abcd 00000000 00000000 00000000 0000efgh

(d | d << 14)
00000000 00000000 00000000 0000abcd 00000000 00000000 00000000 0000efgh
00000000 000000ab cd000000 00000000 00000000 000000ef gh000000 00000000

00000000 000000ab cd000000 0000abcd 00000000 000000ef gh000000 0000efgh

& 0x0003000300030003

00000000 000000ab cd000000 0000abcd 00000000 000000ef gh000000 0000efgh
00000000 00000011 00000000 00000011 00000000 00000011 00000000 00000011

00000000 000000ab 00000000 000000cd 00000000 000000ef 00000000 000000gh

(d | d << 7)

00000000 000000ab 00000000 000000cd 00000000 000000ef 00000000 000000gh
0000000a b0000000 0000000c d0000000 0000000e f0000000 0000000g h0000000

0000000a b00000ab 0000000c d00000cd 0000000e f00000ef 0000000g h00000gh

&0x0101010101010101

0000000a b00000ab 0000000c d00000cd 0000000e f00000ef 0000000g h00000gh
00000001 00000001 00000001 00000001 00000001 00000001 00000001 00000001

0000000a 0000000b 0000000c 0000000d 0000000e 0000000f 0000000g 0000000h
*/

uint ctrl2 = 1;

uint64_t const mask28[2] = {0x0000000F0000000F, 0x0000000F0000000F};
uint64_t const mask14[2] = {0x0003000300030003, 0x0003000300030003};
uint64_t const mask7[2]  = {0x0101010101010101, 0x0101010101010101};

uint64_t sse_calc_z_ord(uint8_t c[8]) {
    START_MICRO("sse")
    
    uint64_t d[8];
    for(uint8_t i = 0; i < 8; ++i)
        d[i] = c[i];
    
    __m128i m28 = _mm_loadu_si128((__m128i*)mask28);
    __m128i m14 = _mm_loadu_si128((__m128i*)mask14);
    __m128i m7 = _mm_loadu_si128((__m128i*)mask7);
    
    uint64_t res = 0;
    for(uint i = 0; i < 4; ++i) {
        __m128i xy_a = _mm_loadu_si128((__m128i*)(d + 2*i));
        
        __m128i xy28 = _mm_slli_epi64(xy_a, 28);
        __m128i xy_b = _mm_or_si128(  xy_a, xy28);
        __m128i xy_c = _mm_and_si128( xy_b, m28);
        
        __m128i xy14 = _mm_slli_epi64(xy_c, 14);
        __m128i xy_d = _mm_or_si128(  xy_c, xy14);
        __m128i xy_e = _mm_and_si128( xy_d, m14);
        
        __m128i xy7 = _mm_slli_epi64( xy_e, 7);
        __m128i xy_f = _mm_or_si128(  xy_e, xy7);
        __m128i xy_g = _mm_and_si128( xy_f, m7);
        
        _mm_stream_si128((__m128i*)(d + 2*i), xy_g);
        res |= d[2*i] | (d[2*i+1] << 1);
    }
    STOP_MICRO()

    ctrl2 += res;
    return res;
}


int main(int argc, char* argv[]) {
    
    //------------------- demo -------------------
    //~ int32_t c[] = {59, 59999, 527};
    uint8_t c[] = {5, 5, 5, 5, 5, 5, 5, 5};
    std::string color[] = {REDB, GREENB, BLUEB, TURB, REDB, GREENB, BLUEB, TURB};
    
    for(uint i = 0; i < 8; ++i) {
        for(uint j = i; j < 8; ++j) {
            std::cout << " ";
        }
        std::stringstream ss;
        ss << std::bitset<8>(c[i]);
        std::string out = ss.str();
        
        for(uint j = 0; j < 8; ++j) {
            std::cout << color[i] << out[j] << "       ";
        }
        std::cout << NONE << std::endl;
    }
    
    std::stringstream ss;
    ss << std::bitset<64>(calc_z_ord(c));
    
    std::string out = ss.str();
    
    std::cout << " ";
    for(uint i = 0; i < out.length(); ++i) {
        std::cout << color[7-((i)%8)] << out[i];
    }
    std::cout << NONE << std::endl;
    
    sse_calc_z_ord(c);
    
    //------------------- benchmark -------------------
    MEASURE(calc_z_ord(c), "z_order normal")
    MEASURE(sse_calc_z_ord(c), "z_order sse")
    
    P_RESULTS()
    P_MICRO()
    std::cout << ctrl1 << std::endl;
    std::cout << ctrl2 << std::endl;
    
    return 0;
}
