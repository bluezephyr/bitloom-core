enable_language(C)
set(CMAKE_C_STANDARD 99)

add_library(i2c_master
    i2c_master.c
    )

target_include_directories(i2c_master PUBLIC ${BITLOOM_ROOT}/include)
target_include_directories(i2c_master PRIVATE ${BITLOOM_CONFIG})

