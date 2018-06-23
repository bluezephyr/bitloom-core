/*
 * Stub to test the Pin digital IO interface for Bit Loom.
 *
 * Copyright (c) 2018. BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "pin_digital_io_stub.h"
#include <string.h>

#define MAX_NO_OF_PINS 20

typedef struct
{
    uint16_t pin_id;
    bool on;
    bool used;
} pin_state_t;

static pin_state_t pin_list[MAX_NO_OF_PINS];

/*
 * Forward declarations
 */
static int16_t find_empty_slot(void);
static int16_t find_index_for_pin_id(uint16_t pin_id);


/*
 * Stub implementation
 */
bool pin_digital_io_read(uint16_t pin_id)
{
    int16_t index = find_index_for_pin_id(pin_id);
    if(index >= 0)
    {
        return pin_list[index].on;
    }
    return false;
}

void pin_digital_io_write(uint16_t pin_id, bool on)
{
    int16_t index;

    if(find_index_for_pin_id(pin_id) < 0)
    {
        index = find_empty_slot();
    }

    if(index >= 0)
    {
        pin_list[index].pin_id = pin_id;
        pin_list[index].on = on;
        pin_list[index].used = true;
    }
}

/*
 * Functions to control the stub
 */
void pin_digital_io_stub_init(void)
{
    memset(pin_list, 0, sizeof(pin_list));
}

void pin_digital_io_stub_set_pin_state(uint16_t pin_id, bool on)
{
    // Use stub implementation
    pin_digital_io_write(pin_id, on);
}

bool pin_digital_io_stub_get_pin_state(uint16_t pin_id)
{
    // Use stub implementation
    return pin_digital_io_read(pin_id);
}

/*
 * Help functions
 */
static int16_t find_index_for_pin_id(uint16_t pin_id)
{
    int16_t i;
    for(i=0; i<MAX_NO_OF_PINS; i++)
    {
        if(pin_list[i].pin_id == pin_id)
        {
            return i;
        }
    }
    return -1;
}

static int16_t find_empty_slot(void)
{
    int16_t i;
    for(i=0; i<MAX_NO_OF_PINS; i++)
    {
        if( !pin_list[i].used )
        {
            return i;
        }
    }
    return -1;
}
