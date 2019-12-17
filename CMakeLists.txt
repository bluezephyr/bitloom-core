cmake_minimum_required(VERSION 3.12)
project(bitloom_core)

# The default setting is to expect that all libs are available
# at the same folder level as the application.  This can be
# changed by setting the cache variable BITLOOM_ROOT
# (cmake -D BITLOOM_ROOT=<path> ..)
# Cache variables can be viewed by "cmake -L"
set(BITLOOM_ROOT "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "Root folder for Bitloom libs")

# BITLOOM_CONFIG should be set to the directory containing the configuration
# for the project's target environment as well as the processor specifics.
set(BITLOOM_CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/tests)

add_subdirectory(src/scheduler)
add_subdirectory(src/i2c_master)

option(COMPILE_TESTS "Compile the tests" OFF)
if (COMPILE_TESTS)
    add_compile_options(-Wall -Wextra -Wpedantic -DUNIT_TEST)
    enable_testing()
    add_subdirectory(tests)
endif(COMPILE_TESTS)

