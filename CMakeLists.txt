cmake_minimum_required (VERSION 2.6)
project(mc_potts)

#=================== compiler & flags ===================
#~ SET(CMAKE_CXX_COMPILER "g++4.7" CACHE STRING "the compiler")
SET(CMAKE_CXX_FLAGS "-Wall -std=c++11 -O3 -pedantic -fno-tree-vectorize")

#=================== set paths ===================
set(SRC "${PROJECT_SOURCE_DIR}/src")
set(TEST "${PROJECT_SOURCE_DIR}/test")
set(EXAMPLE "${PROJECT_SOURCE_DIR}/example")

#=================== set options ===================
OPTION(BUILD_TEST "build all the tests" ON)
OPTION(BUILD_EXAMPLE "build all the examples" ON)
SET(USE_LENGTH 10 CACHE STRING "length of system")
SET(USE_HEIGHT 10 CACHE STRING "height of system")
SET(USE_DEPTH 10 CACHE STRING "depth of system")

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
    
#=================== include directories ===================
include_directories("${SRC}")

#=================== added subdirs ===================
#~ add_subdirectory("")

if(BUILD_TEST)
    enable_testing()
    add_subdirectory("test")
endif(BUILD_TEST)

if(BUILD_EXAMPLE)
    add_subdirectory("examples")
endif(BUILD_EXAMPLE)

