cmake_minimum_required(VERSION 3.14)
project(lab1)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_C_COMPILER /usr/bin/gcc)
SET(CMAKE_CXX_COMPILER /usr/bin/g++)
include_directories(.)
include_directories(lib)
include_directories(tests)

add_executable(lab1
        src/pack.c
        src/pack.h
        src/posit.cpp
        src/posit.h
        src/posit_types.h
        src/util.c
        src/util.h
        tests/test_dot_product_posit.cpp)
