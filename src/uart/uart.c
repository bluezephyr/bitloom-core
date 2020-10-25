/*
 * UART Driver
 *
 * Copyright (c) 2020. BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "hal/uart_hal.h"
#include "bytebuffer.h"

static uint8_t inBufferData[INBUFFER_DATA_SIZE];
static uint8_t outBufferData[OUTBUFFER_DATA_SIZE];

typedef struct
{
    bytebuffer_t inBuffer;
    bytebuffer_t outBuffer;
} uart_t;
static uart_t self;

void uart_init (void)
{
    bytebuffer_init(&self.inBuffer, inBufferData, INBUFFER_DATA_SIZE);
    bytebuffer_init(&self.outBuffer, outBufferData, OUTBUFFER_DATA_SIZE);
    uart_hal_init(&self.inBuffer, &self.outBuffer);
}

int8_t uart_read (uint8_t* buffer, int8_t nbytes)
{
    (void)nbytes;
    if (!bytebuffer_isEmpty(&self.inBuffer))
    {
        *buffer = bytebuffer_read(&self.inBuffer);
        return 1;
    }

    return 0;
}

int8_t uart_write (uint8_t* buffer, int8_t nbytes)
{
    // Calculate available space in the send buffer
    int8_t bytesToWrite = bytebuffer_getSpace(&self.outBuffer);
    int8_t i;

    if (bytesToWrite > nbytes)
    {
        bytesToWrite = nbytes;
    }

    for (i=0; i<bytesToWrite; i++)
    {
        bytebuffer_write(&self.outBuffer, buffer[i]);
    }

    if (bytesToWrite > 0)
    {
        uart_hal_send();
    }

    return bytesToWrite;
}
