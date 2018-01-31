/*
 * Implementation of the timer module for the unit tests.
 * The module implements two interfaces:
 *
 *  - The timer interface needed by the scheduler
 *  - An interface for the test cases to control the timer
 *
 * Copyright (c) 2018. BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "fake_timer.h"

static int m_state;

/*
 * Implementation of the timer interface
 */
void timer_init (void)
{
    m_state = TIMER_DISABLED;
}

void timer_start (void)
{
    m_state = TIMER_ENABLED;
}

void timer_stop (void)
{
    m_state = TIMER_DISABLED;
}


/*
 * Functions to control the fake timer
 */

int faketimer_get_state (void)
{
    return m_state;
}
