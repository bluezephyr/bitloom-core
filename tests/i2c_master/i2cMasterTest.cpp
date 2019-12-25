/*
 * Unit tests for the Bit Loom I2C Master driver.
 *
 * Copyright (c) 2018-2020. BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTestExt/MockSupport.h>

extern "C"
{
    #include "core/i2c_master.h"
    #include "hal/i2c.h"
}

/********************************************************************
 * Defines for a test I2C device
 ********************************************************************/
#define TASK_ID           1
#define DEVICE_ADDRESS    0x3C
#define WRITE_ADDRESS     0x3C
#define READ_ADDRESS      0x3D
#define READ_REGISTER     0x25
#define READ_DATA_BYTE_0  0x30
#define READ_DATA_BYTE_1  0x31
#define READ_DATA_BYTE_2  0x32
#define WRITE_REGISTER    0x20
#define WRITE_DATA_BYTE_0 0x40
#define NACK              0u
#define ACK               1u


TEST_GROUP(i2c_master)
{
    void setup() override
    {
        i2c_master_init(TASK_ID);
    }

    void teardown() override
    {
        mock().checkExpectations();
        mock().clear();
    }

    static void check_driver_idle()
    {
        LONGS_EQUAL(i2c_idle, i2c_master_get_state());
    }

    static void check_driver_busy()
    {
        LONGS_EQUAL(i2c_busy, i2c_master_get_state());
    }

    static void run_number_of_loops(uint16_t number_of_loops)
    {
        uint16_t i;
        for(i=0; i<number_of_loops; i++)
        {
            i2c_master_run();
        }
    }

    static void start_write_op_and_set_driver_in_busy_state()
    {
        uint8_t buffer[2] = {WRITE_REGISTER, WRITE_DATA_BYTE_0};
        uint16_t buffer_len = 2;

        mock().expectOneCall("i2c_start").andReturnValue(i2c_ok);
        mock().expectOneCall("i2c_write_byte").withParameter("byte", WRITE_ADDRESS).andReturnValue(i2c_ack_received);
        mock().expectOneCall("i2c_write_byte").withParameter("byte", buffer[0]).andReturnValue(i2c_ack_received);
        mock().expectOneCall("i2c_write_byte").withParameter("byte", buffer[1]).andReturnValue(i2c_ack_received);
        mock().expectOneCall("i2c_stop");

        i2c_master_write(DEVICE_ADDRESS, buffer, buffer_len);
        run_number_of_loops(3);
    }

    static void run_until(i2c_master_state_t state, uint16_t max_number_of_loops)
    {
        uint16_t i=0;
        while (i2c_master_get_state() != state)
        {
            if (i++ == max_number_of_loops)
            {
                FAIL("Maximum number of run loops exceeded");
            }
            i2c_master_run();
        }
    }
};

/********************************************************************
 * TEST CASES
 ********************************************************************/
TEST(i2c_master, init)
{
    check_driver_idle();
}

TEST(i2c_master, run_driver_idle)
{
    i2c_master_run();
    check_driver_idle();
}

TEST(i2c_master, write_request_sets_driver_in_busy_state)
{
    i2c_master_write(0, NULL, 0);
    check_driver_busy();
}

TEST(i2c_master, no_device_error_stops_communication)
{
    uint8_t buffer[1] = {WRITE_REGISTER};
    uint16_t buffer_len = 1;

    mock().expectOneCall("i2c_start").andReturnValue(i2c_ok);
    mock().expectOneCall("i2c_write_byte").withParameter("byte", WRITE_ADDRESS).andReturnValue(i2c_nack_received);
    mock().expectOneCall("i2c_stop");
    i2c_master_write(DEVICE_ADDRESS, buffer, buffer_len);
    run_until(i2c_error, 10);
}

TEST(i2c_master, write_register_not_available_stops_communication)
{
    uint8_t buffer[1] = {WRITE_REGISTER};
    uint16_t buffer_len = 1;

    mock().expectOneCall("i2c_start").andReturnValue(i2c_ok);
    mock().expectOneCall("i2c_write_byte").withParameter("byte", WRITE_ADDRESS).andReturnValue(i2c_ack_received);
    mock().expectOneCall("i2c_write_byte").withParameter("byte", WRITE_REGISTER).andReturnValue(i2c_nack_received);
    mock().expectOneCall("i2c_stop");
    i2c_master_write(DEVICE_ADDRESS, buffer, buffer_len);
    run_until(i2c_error, 10);
}

TEST(i2c_master, write_single_register_on_device)
{
    uint8_t buffer[2] = {WRITE_REGISTER, WRITE_DATA_BYTE_0};
    uint16_t buffer_len = 2;

    mock().expectOneCall("i2c_start").andReturnValue(i2c_ok);
    mock().expectOneCall("i2c_write_byte").withParameter("byte", WRITE_ADDRESS).andReturnValue(i2c_ack_received);
    mock().expectOneCall("i2c_write_byte").withParameter("byte", buffer[0]).andReturnValue(i2c_ack_received);
    mock().expectOneCall("i2c_write_byte").withParameter("byte", buffer[1]).andReturnValue(i2c_ack_received);
    mock().expectOneCall("i2c_stop");
    i2c_master_write(DEVICE_ADDRESS, buffer, buffer_len);
    run_until(i2c_idle, 10);
}

TEST(i2c_master, write_not_accepted_in_busy_state)
{
    uint8_t buffer[1];
    uint16_t buffer_len = 1;

    start_write_op_and_set_driver_in_busy_state();

    // The following write should not generate any operations
    i2c_master_write(DEVICE_ADDRESS, buffer, buffer_len);
    run_until(i2c_idle, 10);
}

TEST(i2c_master, successful_write_after_failure)
{
    uint8_t buffer[2] = {WRITE_REGISTER, WRITE_DATA_BYTE_0};
    uint16_t buffer_len = 2;

    mock().expectOneCall("i2c_start").andReturnValue(i2c_ok);
    mock().expectOneCall("i2c_write_byte").withParameter("byte", WRITE_ADDRESS).andReturnValue(i2c_nack_received);
    mock().expectOneCall("i2c_stop");
    mock().expectOneCall("i2c_start").andReturnValue(i2c_ok);
    mock().expectOneCall("i2c_write_byte").withParameter("byte", WRITE_ADDRESS).andReturnValue(i2c_ack_received);
    mock().expectOneCall("i2c_write_byte").withParameter("byte", buffer[0]).andReturnValue(i2c_ack_received);
    mock().expectOneCall("i2c_write_byte").withParameter("byte", buffer[1]).andReturnValue(i2c_ack_received);
    mock().expectOneCall("i2c_stop");

    i2c_master_write(DEVICE_ADDRESS, buffer, buffer_len);
    run_until(i2c_error, 10);
    i2c_master_write(DEVICE_ADDRESS, buffer, buffer_len);
    run_until(i2c_idle, 10);
}

TEST(i2c_master, read_not_accepted_in_busy_state)
{
    uint8_t read_buffer[1];
    uint16_t buffer_len = 1;

    start_write_op_and_set_driver_in_busy_state();

    // The following read should not generate any operations
    i2c_master_read_register(DEVICE_ADDRESS, READ_REGISTER, read_buffer, buffer_len);
    run_until(i2c_idle, 10);
}

TEST(i2c_master, read_one_byte_from_register)
{
    uint8_t read_buffer[1];
    uint16_t buffer_len = 1;

    mock().expectOneCall("i2c_start").andReturnValue(i2c_ok);
    mock().expectOneCall("i2c_write_byte").withParameter("byte", WRITE_ADDRESS).andReturnValue(i2c_ack_received);
    mock().expectOneCall("i2c_write_byte").withParameter("byte", READ_REGISTER).andReturnValue(i2c_ack_received);
    mock().expectOneCall("i2c_restart").andReturnValue(i2c_ok);
    mock().expectOneCall("i2c_write_byte").withParameter("byte", READ_ADDRESS).andReturnValue(i2c_ack_received);
    mock().expectOneCall("i2c_read_byte").withParameter("send_ack", NACK).andReturnValue(READ_DATA_BYTE_0);
    mock().expectOneCall("i2c_stop");

    i2c_master_read_register(DEVICE_ADDRESS, READ_REGISTER, read_buffer, buffer_len);
    run_until(i2c_idle, 10);
    UNSIGNED_LONGS_EQUAL(READ_DATA_BYTE_0, read_buffer[0]);
}

TEST(i2c_master, read_several_bytes_from_register)
{
    uint8_t read_buffer[3];
    uint16_t buffer_len = 3;

    mock().expectOneCall("i2c_start").andReturnValue(i2c_ok);
    mock().expectOneCall("i2c_write_byte").withParameter("byte", WRITE_ADDRESS).andReturnValue(i2c_ack_received);
    mock().expectOneCall("i2c_write_byte").withParameter("byte", READ_REGISTER).andReturnValue(i2c_ack_received);
    mock().expectOneCall("i2c_restart").andReturnValue(i2c_ok);
    mock().expectOneCall("i2c_write_byte").withParameter("byte", READ_ADDRESS).andReturnValue(i2c_ack_received);
    mock().expectOneCall("i2c_read_byte").withParameter("send_ack", ACK).andReturnValue(READ_DATA_BYTE_0);
    mock().expectOneCall("i2c_read_byte").withParameter("send_ack", ACK).andReturnValue(READ_DATA_BYTE_1);
    mock().expectOneCall("i2c_read_byte").withParameter("send_ack", NACK).andReturnValue(READ_DATA_BYTE_2);
    mock().expectOneCall("i2c_stop");

    i2c_master_read_register(DEVICE_ADDRESS, READ_REGISTER, read_buffer, buffer_len);
    run_until(i2c_idle, 10);
    UNSIGNED_LONGS_EQUAL(read_buffer[0], READ_DATA_BYTE_0);
    UNSIGNED_LONGS_EQUAL(read_buffer[1], READ_DATA_BYTE_1);
    UNSIGNED_LONGS_EQUAL(read_buffer[2], READ_DATA_BYTE_2);
}

/********************************************************************
 * TEST RUNNER
 ********************************************************************/
int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
