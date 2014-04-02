// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    26.03.2014 15:36:15 CET
// File:    debug.hpp

#ifndef __ADDON_DEBUG_HEADER
#define __ADDON_DEBUG_HEADER

#include "color.hpp"

#define DEBUG_VAR(x) std::cout << REDB << "  DEBUG_VAR: " << RED << #x << " = " << x << NONE << std::endl;
#define DEBUG_MSG(x) std::cout << REDB << "  DEBUG_MSG: " << RED << x << NONE << std::endl;
#define WARNING(x)   std::cout << YELLOWB << "  WARNING: " << YELLOW << x << NONE << std::endl;
#define STRING2(x)   #x
#define STRING(x)    STRING2(x)

#endif //__ADDON_DEBUG_HEADER
