/*
 * Fake timer for the scheduler unit tests.
 *
 * Copyright (c) 2016-2018 BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#ifndef BL_FAKE_TIMER_H
#define BL_FAKE_TIMER_H

#include "timer.h"

enum
{
    TIMER_DISABLED = 0,
    TIMER_ENABLED = 1
};

int faketimer_get_state (void);

#endif // BL_FAKE_TIMER_H
