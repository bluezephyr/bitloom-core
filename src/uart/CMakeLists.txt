add_library(uart
    uart.c
    )

target_include_directories(uart PRIVATE ${BITLOOM_CORE}/include)
target_include_directories(uart PRIVATE ${BITLOOM_CONFIG})
target_include_directories(uart PRIVATE ${CUTIL}/include)

