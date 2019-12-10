cmake_minimum_required(VERSION 3.0)

project(posit LANGUAGES C)
add_executable(posit posit.c)

target_link_libraries(posit m)