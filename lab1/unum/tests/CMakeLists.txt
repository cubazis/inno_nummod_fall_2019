include_directories(..)
include_directories(${CHECK_INCLUDE_DIRS})
find_package(Check)

set(tests
        test_dot_product
        test_dot_product_posit)

foreach (test ${tests})
    add_executable(${test} ${test}.c)
    target_link_libraries(${test} unum ${CHECK_LIBRARIES} m pthread)
    add_test(${test} ${CMAKE_CURRENT_BINARY_DIR}/${test})
endforeach ()
