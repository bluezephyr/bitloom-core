#ifndef UART_CONFIG_H
#define UART_CONFIG_H

/*
 * The UART module uses two byte buffers for incoming and outgoing data.  The sizes of the buffers
 * must be specified.  The buffers are implemented using the bytebuffer module in cutil.  Hence,
 * the sizes must be a power of two.  Maximum size is 128.
 */

#define INBUFFER_DATA_SIZE  <set-value>
#define OUTBUFFER_DATA_SIZE <set-value>

#endif  // UART_CONFIG_H
