/*
 * Interface to access digital IO pins in the Hardware abstraction layer (HAL)
 * for BitLoom.
 *
 * Copyright (c) 2016-2018 BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#ifndef BL_HAL_PIN_DIGITAL_IO_H
#define BL_HAL_PIN_DIGITAL_IO_H

#include <stdbool.h>

/*
 * Read the value of the specified PIN.  True indicates PIN high and False
 * indicates PIN low.
 */
bool pin_digital_io_read(uint16_t pin_id);

/*
 * Write a value to the specified PIN.
 */
void pin_digital_io_write_high(uint16_t pin_id);
void pin_digital_io_write_low(uint16_t pin_id);

#endif // BL_HAL_PIN_DIGITAL_IO_H
