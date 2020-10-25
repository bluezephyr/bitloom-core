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
#include "config/uart_config.h"
#include "bytebuffer.h"

/*
 * Error codes from UART operations:
 */

/*
 * Function to initialize the UART hardware.  This function must be called prior
 * to any other function is called.
 *
 * The function takes two bytebuffers as input - one for writing outgoing data
 * and one for reading incoming data.  It is up to the caller of the function
 * to prepare the buffers before the init function is called.
 */
void uart_hal_init(bytebuffer_t *inBuffer, bytebuffer_t *outBuffer);

/*
 * Function to inform the HAL that new data is available in the outBuffer.
 */
void uart_hal_send(void);

#endif // BL_HAL_UART_H
