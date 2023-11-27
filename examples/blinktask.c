/*
 * This task toggles a led each time the task's run function is called.
 */

#include "blinktask.h"
#include "config.h"

// This id is given by the scheduler when the task is added.
static uint8_t blink_task_id = 0;

void blink_task_init (uint8_t taskid)
{
    // Set the task id
    blink_task_id = taskid;

    // Initialize the LED.
    LED_DDR |= (1 << LED);
    LED_PORT &= ~(1 << LED);
}

void blink_task_run (void)
{
    LED_PORT ^= (1 << LED);
}

