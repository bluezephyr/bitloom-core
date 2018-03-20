/*
 * Simple I2C master driver for BitLoom.
 * 
 * This module implements a simple I2C master driver.  The driver implements
 * one task that conforms to the requirements of the BitLoom scheduler.  No
 * interrupts are used, the communication is done using busy wait.
 *
 * TODO:
 * - Add an error status value and two functions: get_error and reset.  The I2C
 *  functionality will not be resumed until the reset function has been called.
 *
 * - Add current user of the driver. Store this value if an error occurs to
 *   make it possible to evaluate which I2C communication that failed.
 * 
 * Copyright (c) 2016-2018 BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#ifndef BL_I2C_MASTER_H
#define BL_I2C_MASTER_H

#include "i2c_master_config.h"

#endif // BL_I2C_MASTER_H 
