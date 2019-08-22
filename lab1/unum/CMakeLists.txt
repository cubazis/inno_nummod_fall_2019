cmake_minimum_required(VERSION 3.0)

project(unum LANGUAGES C)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -Wshadow")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g")

option( BUILD_TESTS			  "Build tests"							OFF )

set(CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_C_FLAGS_DEBUG
        "${CMAKE_C_FLAGS_DEBUG} -Wall -Wshadow -Wpointer-arith -Wcast-qual -Winline -Werror -Wextra -Wfatal-errors -Wstrict-prototypes")
set(CMAKE_C_FLAGS_RELEASE
        "${CMAKE_C_FLAGS_RELEASE}")

file(GLOB UNUM_SOURCES src/*.c)
file(GLOB UNUM_HEADERS src/*.h)

set(LIB_INSTALL_DIR "lib" CACHE PATH "Where to install libraries to.")
set(CONFIG_INSTALL_DIR "lib/cmake/${PROJECT_NAME}" CACHE PATH "Where to install configs to.")
set(INCLUDE_INSTALL_DIR "include" CACHE PATH "Where to install headers to.")

add_library(unum ${UNUM_SOURCES})

target_include_directories(unum PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
        "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>")
set_target_properties(unum PROPERTIES PUBLIC_HEADER "${UNUM_HEADERS}")


include(CMakePackageConfigHelpers)

configure_package_config_file(cmake/unumConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/unum/unumConfig.cmake
        INSTALL_DESTINATION "${CONFIG_INSTALL_DIR}")

install(TARGETS "unum"
        EXPORT unumTargets
        LIBRARY DESTINATION "${LIB_INSTALL_DIR}"
        ARCHIVE DESTINATION "${LIB_INSTALL_DIR}"
        PUBLIC_HEADER DESTINATION "${INCLUDE_INSTALL_DIR}/unum"
        )

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/unum/unumConfig.cmake
        DESTINATION ${CONFIG_INSTALL_DIR})

install(
        EXPORT unumTargets
        FILE unumTargets.cmake
        DESTINATION ${CONFIG_INSTALL_DIR})

# Testing
if(BUILD_TESTS)
    find_package(Check)
    if(CHECK_FOUND)
        enable_testing()
        add_subdirectory(tests)
    else()
        message("Could not find Check for testing")
    endif()
endif()
