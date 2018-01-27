/*
 * Timer in the Hardware abstraction layer (HAL) for BitLoom.
 *
 * Copyright (c) 2016-2018 BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#ifndef BL_HAL_TIMER_H
#define BL_HAL_TIMER_H

void timer_init (void);
void timer_enable_interrupts (void);
void timer_disable_interrupts (void);

#endif // BL_HAL_TIMER_H
