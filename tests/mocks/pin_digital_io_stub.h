/*
 * Stub to test the Pin digital IO interface for Bit Loom.
 *
 * Copyright (c) 2018 BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#ifndef BL_PIN_DIGITAL_IO_STUB_H
#define BL_PIN_DIGITAL_IO_STUB_H

#include <stdint.h>
#include <stdbool.h>

/*
 * Functions to control the stub
 */
void pin_digital_io_stub_init(void);
void pin_digital_io_stub_set_pin_state_high(uint16_t pin_id);
void pin_digital_io_stub_set_pin_state_low(uint16_t pin_id);
bool pin_digital_io_stub_get_pin_state(uint16_t pin_id);

#endif // BL_PIN_DIGITAL_IO_STUB_H
