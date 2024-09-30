/**\
 * Copyright (c) 2022 Bosch Sensortec GmbH. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 **/

#include "bmp_390_communications.hpp"

#include <BMP3_SensorAPI/bmp3.h>
#include <BMP3_SensorAPI/bmp3_defs.h>
#include <hardware/gpio.h>
#include <hardware/i2c.h>
#include <pico/stdlib.h>

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "board_descriptions.hpp"

namespace {

/* Variable to store the device address */
constexpr std::uint8_t dev_addr{BMP3_ADDR_I2C_SEC};

bool is_initialized{};

}  // namespace

namespace altimeter {

BMP3_INTF_RET_TYPE bmp3_i2c_read(std::uint8_t reg_addr, std::uint8_t* reg_data,
                                 std::uint32_t len, void* intf_ptr) {
  if (!intf_ptr) {
    printf("bmp3_i2c_read: cannot read with intf_ptr equal to nullptr.\n");
    return BMP3_E_COMM_FAIL;
  }

  const auto device_addr{*static_cast<std::uint8_t*>(intf_ptr)};

  // Write register address
  if (i2c_write_blocking(board_descriptions::I2C_PORT, device_addr, &reg_addr,
                         1, true) < 0) {
    return BMP3_E_COMM_FAIL;
  }

  // Read data from register
  if (i2c_read_blocking(board_descriptions::I2C_PORT, device_addr, reg_data,
                        len, false) < 0) {
    return BMP3_E_COMM_FAIL;
  }
  return BMP3_OK;
}

BMP3_INTF_RET_TYPE bmp3_i2c_write(std::uint8_t reg_addr,
                                  const std::uint8_t* reg_data,
                                  std::uint32_t len, void* intf_ptr) {
  if (!intf_ptr) {
    printf("bmp3_i2c_write: cannot write with intf_ptr equal to nullptr.\n");
    return BMP3_E_COMM_FAIL;
  }

  const auto device_addr{*static_cast<uint8_t*>(intf_ptr)};
  std::array<std::uint8_t, 256> buffer;

  // Prepare write buffer
  buffer[0] = reg_addr;
  memcpy(&buffer[1], reg_data, len);

  // Write data to register
  if (i2c_write_blocking(board_descriptions::I2C_PORT, device_addr,
                         buffer.data(), len + 1, false) < 0) {
    return BMP3_E_COMM_FAIL;
  }
  return BMP3_OK;
}

void bmp3_delay_us(std::uint32_t period, void* /* intf_ptr */) {
  sleep_us(period);
}

void bmp3_check_rslt(const char api_name[], std::int8_t rslt) {
  switch (rslt) {
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
      printf("API [%s] Error [%d] : Incorrect length parameter\r\n", api_name,
             rslt);
      break;
    case BMP3_E_DEV_NOT_FOUND:
      printf("API [%s] Error [%d] : Device not found\r\n", api_name, rslt);
      break;
    case BMP3_E_CONFIGURATION_ERR:
      printf("API [%s] Error [%d] : Configuration Error\r\n", api_name, rslt);
      break;
    case BMP3_W_SENSOR_NOT_ENABLED:
      printf("API [%s] Error [%d] : Warning when Sensor not enabled\r\n",
             api_name, rslt);
      break;
    case BMP3_W_INVALID_FIFO_REQ_FRAME_CNT:
      printf(
          "API [%s] Error [%d] : Warning when Fifo watermark level is not in "
          "limit\r\n",
          api_name, rslt);
      break;
    default:
      printf("API [%s] Error [%d] : Unknown error code\r\n", api_name, rslt);
      break;
  }
}

BMP3_INTF_RET_TYPE bmp3_interface_init(bmp3_dev* bmp3, std::uint8_t intf) {
  if (!bmp3) {
    return BMP3_E_NULL_PTR;
  }

  if (!is_initialized) {
    // Initialize I2C
    i2c_init(board_descriptions::I2C_PORT, 100 * 1'000);  // 100kHz frequency
    gpio_set_function(board_descriptions::BMP390_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(board_descriptions::BMP390_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(board_descriptions::BMP390_SDA_PIN);
    gpio_pull_up(board_descriptions::BMP390_SCL_PIN);
    is_initialized = true;
  }

  /* Bus configuration : I2C */
  bmp3->delay_us = bmp3_delay_us;
  if (intf == BMP3_I2C_INTF) {
    printf("I2C Interface\n");
    bmp3->intf_ptr = const_cast<void*>(static_cast<const void*>(&dev_addr));
    bmp3->read = bmp3_i2c_read;
    bmp3->write = bmp3_i2c_write;
    bmp3->intf = BMP3_I2C_INTF;
  }
  return BMP3_OK;
}

void bmp3_deinit() { i2c_deinit(board_descriptions::I2C_PORT); }

}  // namespace altimeter