include_directories(..)
include_directories(${CHECK_INCLUDE_DIRS})

set(tests
  test_dot_product_posit
)

set(CMAKE_EXE_LINKER_FLAGS "${CHECK_LDFLAGS_OTHER} ${CMAKE_EXE_LINKER_FLAGS}")
# Add and register tests
foreach(test ${tests})
  add_executable(${test} ${test}.c)
  target_link_libraries(${test}
    unum
    ${CHECK_LIBRARIES}
    m)
  add_test(${test} ${CMAKE_CURRENT_BINARY_DIR}/${test})
endforeach()
