/* Defines for the scheduler example project. */

#ifndef BOARDCONFIG_H
#define BOARDCONFIG_H

#include <avr/io.h>

// Defines for the scheduler
#define SCHEDULER_NO_TASKS      1

// Defines for the LEDs
#define LED_PORT    PORTB
#define LED_DDR     DDRB
#define LED         PB0

#endif  // BOARDCONFIG_H
