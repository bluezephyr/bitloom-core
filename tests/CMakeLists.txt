message("CPPUTEST_HOME: '${CPPUTEST_HOME}'")
find_library(CPPUTESTLIB NAMES CppUTest HINTS ${CPPUTEST_HOME}/cpputest_build/lib)
find_library(CPPUTESTEXTLIB NAMES CppUTestExt HINTS ${CPPUTEST_HOME}/cpputest_build/lib)
message("CppUTest lib: ${CPPUTESTLIB}")
message("CppUTestExt lib: ${CPPUTESTEXTLIB}")

add_executable(scheduler_test
    scheduler/SchedulerTest.cpp
    mocks/spy_task.c )

target_include_directories(scheduler_test PRIVATE ${CPPUTEST_HOME}/include)
target_include_directories(scheduler_test PRIVATE ${BITLOOM_ROOT}/include)
target_include_directories(scheduler_test PRIVATE ${BITLOOM_CONFIG})
target_include_directories(scheduler_test PRIVATE mocks)

target_link_libraries(scheduler_test
    scheduler
    ${CPPUTESTLIB}
    ${CPPUTESTEXTLIB} )

add_executable(i2c_master_test
    i2c_master/i2cMasterTest.cpp
    mocks/i2c_mock.cpp )

target_link_libraries(i2c_master_test
    i2c_master
    ${CPPUTESTLIB}
    ${CPPUTESTEXTLIB} )

target_include_directories(i2c_master_test PRIVATE ${CPPUTEST_HOME}/include)
target_include_directories(i2c_master_test PRIVATE ${BITLOOM_ROOT}/include)
target_include_directories(i2c_master_test PRIVATE ${BITLOOM_CONFIG})
target_include_directories(i2c_master_test PRIVATE mocks)

add_test(scheduler scheduler_test)
add_test(i2c_master i2c_master_test)
