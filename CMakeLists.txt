cmake_minimum_required(VERSION 3.12)
project(bitloom_core)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

# BITLOOM_CORE must be set to where the bitloom-core repo is located
set(BITLOOM_CORE ${CMAKE_CURRENT_SOURCE_DIR})

# BITLOOM_CONFIG should be set to the directory containing the configuration
# for the project's target environment as well as the processor specifics.
set(BITLOOM_CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/tests)

add_compile_options(-Wall -Wextra -Wpedantic -DUNIT_TEST)
add_subdirectory(src/scheduler)

option(COMPILE_TESTS "Compile the tests" OFF)
if (COMPILE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif(COMPILE_TESTS)

