/**\
 * Copyright (c) 2022 Bosch Sensortec GmbH. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 **/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>

#include "bmp3.h"
#include "bmp3_defs.h"
#include "bmp3_communications.hpp"

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

/*! BMP3 shuttle board ID */
#define BMP3_SHUTTLE_ID  0xD3
#define I2C_PORT i2c0

/* Variable to store the device address */
static uint8_t dev_addr;

/*!
 * I2C read function map to COINES platform
 */
BMP3_INTF_RET_TYPE bmp3_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    uint8_t device_addr = *(uint8_t*)intf_ptr;
    int result;

    // Write register address
    result = i2c_write_blocking(I2C_PORT, device_addr, &reg_addr, 1, true);
    if (result < 0)
        return BMP3_E_COMM_FAIL;

    // Read data from register
    result = i2c_read_blocking(I2C_PORT, device_addr, reg_data, len, false);
    if (result < 0)
        return BMP3_E_COMM_FAIL;

    return BMP3_OK;
}

/*!
 * I2C write function map to COINES platform
 */
BMP3_INTF_RET_TYPE bmp3_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    uint8_t device_addr = *(uint8_t*)intf_ptr;
    int result;
    uint8_t buffer[256];

    // Prepare write buffer
    buffer[0] = reg_addr;
    memcpy(&buffer[1], reg_data, len);

    // Write data to register
    result = i2c_write_blocking(I2C_PORT, device_addr, buffer, len + 1, false);
    if (result < 0)
        return BMP3_E_COMM_FAIL;

    return BMP3_OK;
}

/*!
 * Delay function map to COINES platform
 */
void bmp3_delay_us(uint32_t period, void *intf_ptr)
{
    (void)intf_ptr;
    sleep_us(period);
}

void bmp3_check_rslt(const char api_name[], int8_t rslt)
{
    switch (rslt)
    {
        case BMP3_OK:
            /* Do nothing */
            break;
        case BMP3_E_NULL_PTR:
            printf("API [%s] Error [%d] : Null pointer\r\n", api_name, rslt);
            break;
        case BMP3_E_COMM_FAIL:
            printf("API [%s] Error [%d] : Communication failure\r\n", api_name, rslt);
            break;
        case BMP3_E_INVALID_LEN:
            printf("API [%s] Error [%d] : Incorrect length parameter\r\n", api_name, rslt);
            break;
        case BMP3_E_DEV_NOT_FOUND:
            printf("API [%s] Error [%d] : Device not found\r\n", api_name, rslt);
            break;
        case BMP3_E_CONFIGURATION_ERR:
            printf("API [%s] Error [%d] : Configuration Error\r\n", api_name, rslt);
            break;
        case BMP3_W_SENSOR_NOT_ENABLED:
            printf("API [%s] Error [%d] : Warning when Sensor not enabled\r\n", api_name, rslt);
            break;
        case BMP3_W_INVALID_FIFO_REQ_FRAME_CNT:
            printf("API [%s] Error [%d] : Warning when Fifo watermark level is not in limit\r\n", api_name, rslt);
            break;
        default:
            printf("API [%s] Error [%d] : Unknown error code\r\n", api_name, rslt);
            break;
    }
}

BMP3_INTF_RET_TYPE bmp3_interface_init(struct bmp3_dev *bmp3, uint8_t intf)
{
    int8_t rslt = BMP3_OK;

    if (bmp3 != NULL)
    {
        // Initialize I2C
        i2c_init(I2C_PORT, 100 * 1000);  // 100kHz frequency
        gpio_set_function(12 /* SDA PIN */, GPIO_FUNC_I2C);
        gpio_set_function(13 /* SCL PIN */, GPIO_FUNC_I2C);
        gpio_pull_up(12 /* I2C_SDA_PIN */);
        gpio_pull_up(13 /* I2C_SCL_PIN */);

        /* Bus configuration : I2C */
        if (intf == BMP3_I2C_INTF)
        {
            printf("I2C Interface\n");
            dev_addr = BMP3_ADDR_I2C_SEC;
            bmp3->read = bmp3_i2c_read;
            bmp3->write = bmp3_i2c_write;
            bmp3->intf = BMP3_I2C_INTF;
        }

        bmp3->delay_us = bmp3_delay_us;
        bmp3->intf_ptr = &dev_addr;
    }
    else
    {
        rslt = BMP3_E_NULL_PTR;
    }

    return rslt;
}

void bmp3_coines_deinit(void)
{
    i2c_deinit(I2C_PORT);
}
