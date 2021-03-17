add_library(i2c_master
    i2c_master.c
    )

target_include_directories(i2c_master PUBLIC ${BITLOOM_CORE}/include)
target_include_directories(i2c_master PRIVATE ${BITLOOM_CONFIG})

