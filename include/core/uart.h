/*
 * UART driver interface for BitLoom.
 *
 * Copyright (c) 2020 BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#ifndef BL_UART_H
#define BL_UART_H

#include <stdint.h>
#include "config/uart_config.h"

/*
 * Error codes from UART operations:
 */

/*
 * Function to initialize the UART driver.  This function must be called prior
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
uint16_t uart_read (uint8_t* buffer, uint16_t nbytes);

/*
 * Write data to UART.
 *
 * The operation will write up to the specified bytes (nbytes) from the buffer to UART.
 * The function returns the number of written bytes.  A negative number indicates an error
 * according to the error coded defined.
 */
uint16_t uart_write (uint8_t* buffer, uint16_t nbytes);

#endif // BL_UART_H

