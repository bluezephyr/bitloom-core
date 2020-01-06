/*
 * UART interface in the Hardware abstraction layer (HAL) for BitLoom.
 *
 * Copyright (c) 2020 BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#ifndef BL_HAL_UART_H
#define BL_HAL_UART_H

#include <stdint.h>

/*
 * Error codes from UART operations:
 */

/*
 * Function to initialize the UART hardware.  This function must be called prior
 * to any other function is called.
 */
void uart_init (void);

/*
 * Read data from UART.
 *
 * The operation will read up to the specified bytes (nbytes) from UART to the buffer.
 * The function returns the number of read bytes.  A negative number indicates an error
 * according to the error coded defined.
 *
 * It is up to the caller to allocate memory for the data in the buffer.
 */
int8_t uart_read (uint8_t* buffer, int8_t nbyte);

#endif // BL_HAL_UART_H
