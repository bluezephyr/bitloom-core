/*
 * Timer in the Hardware abstraction layer (HAL) for BitLoom.
 *
 * Copyright (c) 2016-2020 BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#ifndef BL_HAL_TIMER_H
#define BL_HAL_TIMER_H

#include "config/timer_config.h"

/*
 * The init function must be called before the timer can be used, i.e., before the functions
 * start and stop are called.
 */
void timer_init (void);

/*
 * The start function starts the ticking of the timer.
 */
void timer_start (void);

/*
 * The stop function stops the ticking of the timer.
 */
void timer_stop (void);

#endif // BL_HAL_TIMER_H
