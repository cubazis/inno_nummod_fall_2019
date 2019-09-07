cmake_minimum_required(VERSION 3.0)

project(unum LANGUAGES C)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -Wshadow")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g")


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wshadow -Wpointer-arith -Wcast-qual -Winline -Werror -Wextra -Wfatal-errors -Wstrict-prototypes")

file(GLOB UNUM_SOURCES src/*.c)
file(GLOB UNUM_HEADERS src/*.h)

add_library(unum ${UNUM_SOURCES})

target_include_directories(unum PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
        "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>")
set_target_properties(unum PROPERTIES PUBLIC_HEADER "${UNUM_HEADERS}")

add_subdirectory(tests)
