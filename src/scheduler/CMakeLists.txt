cmake_minimum_required(VERSION 3.12)
project(scheduler C)

set(CMAKE_C_STANDARD 99)

add_library(scheduler
        scheduler.c
        ${BITLOOM_ROOT}/bitloom-core/include/core/scheduler.h
        )

target_include_directories(scheduler PUBLIC ${BITLOOM_ROOT}/bitloom-core/include)
target_include_directories(scheduler PRIVATE ${BITLOOM_PROJECT_CONFIG})
