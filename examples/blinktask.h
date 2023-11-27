/*
 * This task toggles a led each time the task's run function is called.
 *
 * The following needs to be set in the boardconfig.h
 *
 * LED_DDR  - DDR for the LED
 * LED_PORT - Port for the LED
 * LED      - The pin where the LED is connected.
 *
 */
#ifndef BLINKTASK_H
#define BLINKTASK_H

#include <avr/io.h>

/*
 * Init function. Must be called before the scheduler is started.
 * The id is the task id and is pro
 */
void blink_task_init(uint8_t taskid);

/*
 * Run function for the task. Called by the scheduler.
 */
void blink_task_run(void);

#endif // BLINKTASK_H
