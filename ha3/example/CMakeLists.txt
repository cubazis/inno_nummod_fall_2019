cmake_minimum_required(VERSION 3.10)
project(example C)

set(CMAKE_C_STANDARD 99)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/data" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(example main.c)