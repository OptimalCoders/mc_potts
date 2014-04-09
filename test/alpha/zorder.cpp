// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    02.04.2014 21:04:00 CEST
// File:    zorder.cpp

#include <iostream>
#include <bitset>

#define BITPRINT(x) std::cout << std::bitset<32>(x) << std::endl; 

uint32_t calc_z_ord(uint8_t const & i, uint8_t const & j, uint8_t const & k) {
    
    static const uint32_t mask[] = {0x00249249, 0x000C30C3, 0x0000F00F};
    static const uint32_t shift[] = {2, 4, 8};
    
    uint32_t x = i;
    uint32_t y = j;
    uint32_t z = k;

    x = (x | (x << shift[2])) & mask[2];
    x = (x | (x << shift[1])) & mask[1];
    x = (x | (x << shift[0])) & mask[0];

    y = (y | (y << shift[2])) & mask[2];
    y = (y | (y << shift[1])) & mask[1];
    y = (y | (y << shift[0])) & mask[0];
    
    z = (z | (z << shift[2])) & mask[2];
    z = (z | (z << shift[1])) & mask[1];
    z = (z | (z << shift[0])) & mask[0];

    return x | (y << 1) | (z << 2);
}

int main(int argc, char* argv[]) {    
    BITPRINT(calc_z_ord(0, 255, 0))
    
    return 0;
}

