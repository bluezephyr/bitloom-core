add_library(scheduler
    scheduler.c
    )

target_include_directories(scheduler PUBLIC ${BITLOOM_CORE}/include)
target_include_directories(scheduler PRIVATE ${BITLOOM_CONFIG})

