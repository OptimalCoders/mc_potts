// Author:  Dominik Gresch <greschd@ethz.ch>
// Date:    18.04.2014 18:26:16 CEST
// File:    validate.cpp

#include <conf.hpp>
#include <beta.hpp>
#include <validation.hpp>

int main(int argc, char* argv[]) {
    using namespace mc_potts;
    validate<SIM_MACRO, SIM2_MACRO, addon::RNG_MACRO, GRID_MACRO, GRID2_MACRO, MATRIX_MACRO, MATRIX2_MACRO>();
    return 0;
}
