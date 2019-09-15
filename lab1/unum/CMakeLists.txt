cmake_minimum_required(VERSION 3.12)
project(unum)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

file(GLOB UNUM_SOURCES src/*.cpp src/*.c)
file(GLOB UNUM_HEADERS src/*.h)
set(UNUM_TESTS tests/test_dot_product_posit.cpp)

add_library(unum ${UNUM_SOURCES})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
if(Boost_FOUND)
    enable_testing()
    include_directories(${Boost_INCLUDE_DIRS})
    set(TEST test_dot_product_posit)
    add_executable(${TEST} ${UNUM_TESTS})
    target_link_libraries(${TEST} unum ${Boost_LIBRARIES})
    add_test(${TEST} ${TEST})
else()
    message("Boost not found")
endif()