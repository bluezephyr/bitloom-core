/*
 * Unit tests for the Bit Loom Pin Digital IO module.  The test cases are only
 * created to test the interface since the actual implementation needs to be
 * done in the adaptation for the specific hardware.
 *
 * Copyright (c) 2018. BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "unity_fixture.h"
#include "pin_digital_io_stub.h"
#include "pin_digital_io.h"

#define PIN_ID_NO_1     (uint16_t)1

TEST_GROUP(pin_digital_io);
TEST_GROUP_RUNNER(pin_digital_io)
{
    RUN_TEST_CASE(pin_digital_io, write_single_pin_true);
    RUN_TEST_CASE(pin_digital_io, write_single_pin_false);
    RUN_TEST_CASE(pin_digital_io, read_single_pin_false);
    RUN_TEST_CASE(pin_digital_io, read_single_pin_true);
}

TEST_SETUP(pin_digital_io)
{
    pin_digital_io_stub_init();
}

TEST_TEAR_DOWN(pin_digital_io)
{
    // Nothing to clean up
}

/*
 * HELP FUNCTIONS
 */
void check_written_pin_state(uint16_t pin_id, bool state)
{
    TEST_ASSERT_EQUAL_UINT16(state, pin_digital_io_stub_get_pin_state(pin_id));
}

void check_pin_state(uint16_t pin_id, bool state)
{
    TEST_ASSERT_EQUAL_UINT16(state, pin_digital_io_read(pin_id));
}

/*
 * TEST CASES
 */
TEST(pin_digital_io, write_single_pin_true)
{
    pin_digital_io_write_high(PIN_ID_NO_1);
    check_written_pin_state(PIN_ID_NO_1, true);
}

TEST(pin_digital_io, write_single_pin_false)
{
    pin_digital_io_write_low(PIN_ID_NO_1);
    check_written_pin_state(PIN_ID_NO_1, false);
}

TEST(pin_digital_io, read_single_pin_false)
{
    pin_digital_io_stub_set_pin_state_low(PIN_ID_NO_1);
    check_pin_state(PIN_ID_NO_1, false);
}

TEST(pin_digital_io, read_single_pin_true)
{
    pin_digital_io_stub_set_pin_state_high(PIN_ID_NO_1);
    check_pin_state(PIN_ID_NO_1, true);
}
