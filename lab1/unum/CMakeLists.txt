cmake_minimum_required(VERSION 3.0)
project(unum C)

set(CMAKE_C_STANDARD 99)

add_library(unum posit.c posit.h)

add_executable(tests_dot tests_dot.c)
target_link_libraries(tests_dot unum)

enable_testing()
add_test(tdot1 tests_dot)
set_tests_properties(tdot1 PROPERTIES
        PASS_REGULAR_EXPRESSION "1"
        FAIL_REGULER_EXPRESSION "0")