cmake_minimum_required (VERSION 2.6)
project(mc_potts)

#=================== including system specifics =========

include("${PROJECT_SOURCE_DIR}/MKL_paths.txt")

#=================== compiler & flags ===================
#~ SET(CMAKE_CXX_COMPILER "gcc-4.8" CACHE STRING "the compiler")
SET(CMACRO "")
SET(COMPILER_FLAGS " -Wall -std=c++11 -O3 -pedantic " CACHE STRING "c++ flags")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILER_FLAGS} ${CMACRO}" CACHE STRING "c++ flags" FORCE)


macro (add_executable _name)
    # invoke built-in add_executable
    _add_executable(${ARGV})
    if (TARGET ${_name})
        target_link_libraries(${_name} mkl_intel_lp64.a mkl_gnu_thread.a mkl_core.a iomp5 m dl pthread gfortran)
    endif()
endmacro()

#=================== set paths ===================
set(SRC "${PROJECT_SOURCE_DIR}/src")
set(TEST "${PROJECT_SOURCE_DIR}/test")
set(EXAMPLE "${PROJECT_SOURCE_DIR}/example")
set(PERFORMANCE "${PROJECT_SOURCE_DIR}/performance")

#=================== set options ===================
OPTION(BUILD_TEST "build all the tests" ON)
OPTION(BUILD_EXAMPLE "build all the examples" ON)
OPTION(BUILD_PERFORMANCE "build performance testing" ON)
OPTION(BUILD_VALIDATION "build validation" ON)
SET(USE_LENGTH 10 CACHE STRING "length of the system")
SET(USE_HEIGHT 10 CACHE STRING "height of the system")
SET(USE_DEPTH 10 CACHE STRING "depth of the system")
SET(USE_TEMP 10 CACHE STRING "temperature of the system")
SET(USE_SIM 10 CACHE STRING "SIM_MACRO")
SET(USE_SIM2 10 CACHE STRING "SIM2_MACRO")
SET(USE_RNG 10 CACHE STRING "RNG_MACRO")
SET(USE_GRID 10 CACHE STRING "GRID_MACRO")
SET(USE_GRID2 10 CACHE STRING "GRID_MACRO")
SET(USE_MATRIX 10 CACHE STRING "MATRIX_MACRO")
SET(USE_MATRIX2 10 CACHE STRING "MATRIX2_MACRO")
SET(MKL_VSL "/opt/intel/composer_xe_2013_sp1.2.144/mkl/include/mkl_vsl.h" CACHE STRING "path of mkl_vsl.h")

#=================== custom stuff ===================
macro(add_default_test)
    set(name ${ARGV0})
    add_test(${name} ${name})
endmacro(add_default_test)

#=================== write config files ===================
configure_file(
    "${SRC}/conf.hpp.in"
    "${SRC}/conf.hpp"
    )
    
configure_file(
    "${SRC}/../addons_python/build_dir.py.in"
    "${SRC}/../addons_python/build_dir.py"
    )
    
#=================== include directories ===================
include_directories("${SRC}")

#=================== added subdirs ===================
#~ add_subdirectory("")

enable_testing()

if(BUILD_EXAMPLE)
    add_subdirectory("examples")
endif(BUILD_EXAMPLE)

if(BUILD_TEST)
    add_subdirectory("test")
endif(BUILD_TEST)

if(BUILD_PERFORMANCE)
    add_subdirectory("performance")
endif(BUILD_PERFORMANCE)

if(BUILD_VALIDATION)
    add_subdirectory("validation")
endif(BUILD_VALIDATION)
