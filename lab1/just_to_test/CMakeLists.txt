cmake_minimum_required(VERSION 3.12)
project(just_to_test C)

set(CMAKE_C_STANDARD 99)

find_package(unum REQUIRED)
include_directories(${UNUM_INCLUDE_DIRS})

add_executable(just_to_test main.c)

target_link_libraries(just_to_test ${UNUM_LIBRARIES})
