cmake_minimum_required(VERSION 3.12)
project(bitloom_core C)

set(CMAKE_C_STANDARD 99)
set(UNITY_HOME $ENV{UNITY_HOME})
set(BITLOOM_ROOT ".." CACHE STRING "Root folder for all Bitloom projects")

include_directories(${UNITY_HOME}/src)
include_directories(${UNITY_HOME}/extras/fixture/src)
add_subdirectory(${BITLOOM_ROOT}/bitloom-costumes libs)

add_compile_options(-Wall -Wextra -Wpedantic)

set(UNITY_FILES
        ${UNITY_HOME}/extras/fixture/src/unity_fixture.c
        ${UNITY_HOME}/src/unity.c
        )

add_executable(bitloom-core-test
        ${UNITY_FILES}
        include/scheduler.h
        src/scheduler/scheduler.c
        include/i2c_master.h
        src/i2c_master/i2c_master.c
        tests/test_main.c
        tests/scheduler/test_fake_timer.c
        tests/scheduler/test_scheduler.c
        tests/scheduler/test_spy_task.c
        tests/pin_digital_io/test_pin_digital_io.c
        tests/i2c_master/test_i2c_master.c
        tests/mocks/fake_timer.c
        tests/mocks/i2c_mock.c
        tests/mocks/pin_digital_io_stub.c
        tests/mocks/spy_task.c
        )

target_include_directories(bitloom-core-test PRIVATE include)
target_include_directories(bitloom-core-test PRIVATE include/hal)
target_include_directories(bitloom-core-test PRIVATE tests/config)
target_include_directories(bitloom-core-test PRIVATE tests/mocks)

target_link_libraries(bitloom-core-test bitloom-costumes)

