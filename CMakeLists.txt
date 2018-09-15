cmake_minimum_required(VERSION 3.12)
project(bitloom_core C)

set(CMAKE_C_STANDARD 99)
set(UNITY_HOME $ENV{UNITY_HOME})

include_directories(${UNITY_HOME}/src)
include_directories(${UNITY_HOME}/extras/fixture/src)

include_directories(include)
include_directories(include/hal)
include_directories(tests/config)
include_directories(tests/mocks)

set (HEADER_FILES
        include/i2c_master.h
        include/scheduler.h
        include/hal/i2c.h
        include/hal/pin_digital_io.h
        include/hal/timer.h
        tests/config/i2c_master_config.h
        tests/config/scheduler_config.h
        tests/mocks/fake_timer.h
        tests/mocks/i2c_mock.h
        tests/mocks/pin_digital_io_stub.h
        tests/mocks/spy_task.h
        )

set(SOURCE_FILES
        ${UNITY_HOME}/extras/fixture/src/unity_fixture.c
        ${UNITY_HOME}/src/unity.c
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
        src/i2c_master/i2c_master.c
        src/scheduler/scheduler.c
        )

add_executable(unit_tests ${SOURCE_FILES} ${HEADER_FILES})
