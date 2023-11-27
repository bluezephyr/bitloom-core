/*
 * Example for the AVR simple scheduler.
 *
 * The program configures one task that blinks a LED at a specified interval.
 *
 * Copyright 2016. BlueZephyr Design.
 */

#include <avr/io.h>
#include <avr/sleep.h>
#include "config.h"
#include "blinktask.h"
#include "scheduler.h"

int main (void)
{
    uint8_t taskid;

    // Init the scheduler
    schedule_init ();

    // Add the task to the scheduler
    // Period 250, offset 0
    taskid = schedule_add_task (250, 0, blink_task_run);

    // Init the task (provide the taskid)
    blink_task_init (taskid);

    // Start the scheduler
    schedule_start ();

    // Mainloop
    while (1)
    {
        schedule_run ();
        set_sleep_mode(SLEEP_MODE_IDLE);
        sleep_mode();
    }
    return (0);
}
