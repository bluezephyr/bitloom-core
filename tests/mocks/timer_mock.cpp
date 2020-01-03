/*
 * Implementation of the timer module for the unit tests.
 * The module implements a mock timer that can be controlled by the test cases
 * to control the ticks for the scheduler.
 *
 * The implementation uses the CppUMock framework
 *
 * Copyright (c) 2020. BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include <CppUTestExt/MockSupport.h>

extern "C"
{
    // This module mocks the following interface
    #include "hal/timer.h"
}

static Tick_t tick;

void timer_init(void)
{
    tick = 0;
}

void timer_start(void)
{
    mock().actualCall("timer_start");
}

Tick_t timer_get_ticks(void)
{
    return tick++;
}

