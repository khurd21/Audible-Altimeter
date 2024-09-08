/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <array>
#include <vector>
#include <string_view>
#include <cstdint>
#include <cctype>
#include <ranges>
#include <algorithm>
#include <utility>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"

#include "bmp3.h"
#include "bmp3_defs.h"

#define I2C_PORT i2c0

namespace {

bmp3_dev bmp3;
constexpr std::uint32_t DOT_PERIOD_MS{ 100 };

namespace Tiny2350 {

constexpr std::uint32_t LED_PIN_RED{ 18 };
constexpr std::uint32_t LED_PIN_GREEN{ 19 };
constexpr std::uint32_t LED_PIN_BLUE{ 20 };

constexpr std::array<std::uint32_t, 3> led_pins{
    LED_PIN_BLUE,
    LED_PIN_GREEN,
    LED_PIN_RED,
};

}

namespace BMP390 {

constexpr std::uint32_t SDA_PIN{ 12 };
constexpr std::uint32_t SCL_PIN{ 13 };
constexpr std::uint32_t ADDRESS{ 0x77 };

}

auto init_i2c() {
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(BMP390::SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(BMP390::SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(BMP390::SDA_PIN);
    gpio_pull_up(BMP390::SCL_PIN);
}

std::int8_t i2c_write(std::uint8_t reg_addr, const std::uint8_t *reg_data, std::uint32_t len, void * /* intf_ptr */) {
    if (i2c_write_blocking(I2C_PORT, BMP390::ADDRESS, &reg_addr, 1, true) == PICO_ERROR_GENERIC ||
        i2c_write_blocking(I2C_PORT, BMP390::ADDRESS, reg_data, len, false) == PICO_ERROR_GENERIC) {
        printf("Pico error when writing i2c.\n");
        return 1;
    }
    return 0;
}

std::int8_t i2c_read(std::uint8_t reg_addr, std::uint8_t *reg_data, std::uint32_t len, void* /* intf_ptr */) {
    if (i2c_write_blocking(I2C_PORT, BMP390::ADDRESS, &reg_addr, 1, true) == PICO_ERROR_GENERIC ||
        i2c_read_blocking(I2C_PORT, BMP390::ADDRESS, reg_data, len, false) == PICO_ERROR_GENERIC) {
        printf("PICO error when reading i2c.\n");
        return 1;
    }
    return 0;
}

auto delay_us(std::uint32_t us, void* /* intf_ptr */) {
    sleep_us(us);
}

std::int8_t bmp3_initialize_sensor() {

    // This greatly copies the init method in the Adafruit_BMP3XX library (added as submodule.)
    std::int8_t rslt;
    static uint8_t dummy; // A dummy variable to act as a valid pointer

    // Assign the I2C or SPI interface to the bmp3 structure
    bmp3.intf = BMP3_I2C_INTF;  // Use BMP3_SPI_INTF for SPI
    bmp3.chip_id = BMP390_CHIP_ID;
    bmp3.read = i2c_read;  // Function pointer for I2C read
    bmp3.write = i2c_write;  // Function pointer for I2C write
    bmp3.delay_us = delay_us;  // Function pointer for delay
    bmp3.intf_ptr = &dummy;

    // Reset the sensor
    rslt = bmp3_soft_reset(&bmp3);
    if (rslt != BMP3_OK) {
        printf("Failed to soft reset BMP3 sensor.\n");
        return rslt;
    }

    // Initialize the sensor
    rslt = bmp3_init(&bmp3);
    if (rslt != BMP3_OK) {
        printf("Failed to initialize BMP3 sensor.\n");
        return rslt;
    }
    
    // Configure desired sensor settings
    uint32_t settings_sel;
    settings_sel = BMP3_SEL_PRESS_EN | BMP3_SEL_TEMP_EN | BMP3_SEL_ODR;
    bmp3.settings.odr_filter.press_os = BMP3_ENABLE;
    bmp3.settings.odr_filter.temp_os = BMP3_ENABLE;
    bmp3.settings.odr_filter.odr = BMP3_ODR_50_HZ;
    bmp3.settings.op_mode = BMP3_MODE_FORCED;

    // Set sensor settings
    rslt = bmp3_set_sensor_settings(settings_sel, &bmp3);
    if (rslt != BMP3_OK) {
        printf("Failed to set sensor settings.");
        return rslt;
    }
    
    return rslt;
}

auto turn_leds_off() {
    gpio_put(Tiny2350::LED_PIN_RED, true);
    gpio_put(Tiny2350::LED_PIN_GREEN, true);
    gpio_put(Tiny2350::LED_PIN_BLUE, true);
}

constexpr auto SEA_LEVEL_PRESSURE = 101325.0f;  // Standard sea-level pressure in Pa

auto calculate_altitude(float current_pressure, float reference_pressure) {
    return ((reference_pressure - current_pressure) / reference_pressure) * 44330.0;
}

auto meters_to_feet(float meters) {
    return meters * 3.28084f;
}

auto performReading(bmp3_data* data) {
  int8_t rslt;
  /* Used to select the settings user needs to change */
  uint16_t settings_sel = 0;
  /* Variable used to select the sensor component */
  uint8_t sensor_comp = 0;

  /* Select the pressure and temperature sensor to be enabled */
  bmp3.settings.temp_en = BMP3_ENABLE;
  settings_sel |= BMP3_SEL_TEMP_EN;
  sensor_comp |= BMP3_TEMP;

  bmp3.settings.press_en = BMP3_ENABLE;
  settings_sel |= BMP3_SEL_PRESS_EN;
  sensor_comp |= BMP3_PRESS;

  // set interrupt to data ready
  // settings_sel |= BMP3_DRDY_EN_SEL | BMP3_LEVEL_SEL | BMP3_LATCH_SEL;

  rslt = bmp3_set_sensor_settings(settings_sel, &bmp3);

  if (rslt != BMP3_OK)
    return false;

  /* Set the power mode */
  bmp3.settings.op_mode = BMP3_MODE_FORCED;
  rslt = bmp3_set_op_mode(&bmp3);
  if (rslt != BMP3_OK)
    return false;

  /* Temperature and Pressure data are read and stored in the bmp3_data instance
   */
  rslt = bmp3_get_sensor_data(sensor_comp, data, &bmp3);
  if (rslt != BMP3_OK)
    return false;

  /* Save the temperature and pressure data */
  return true;
}

}

int main() {
    stdio_init_all();
    std::ranges::for_each(std::as_const(Tiny2350::led_pins), [](const auto& pin) {
        gpio_init(pin);
    });
    std::ranges::for_each(std::as_const(Tiny2350::led_pins), [](const auto& pin) {
        gpio_set_dir(pin, GPIO_OUT);
    });
    sleep_ms(10'000);
    turn_leds_off();

    init_i2c();  // Initialize I2C
    if (bmp3_initialize_sensor() != 0) {
        printf("Issue initializing BMP390 sensor.\n");
    }

    float reference_pressure = 0.0f;
    bool reference_set = false;

    while (true) {
        bmp3_data data;
        if (!performReading(&data)) {
            printf("Error reading data.\n");
        }

        if (!reference_set) {
            reference_pressure = data.pressure;
            reference_set = true;
        }

        const auto altitude_m = calculate_altitude(data.pressure, reference_pressure);
        const auto altitude_ft = meters_to_feet(altitude_m);
        printf("Temperature: %.2f C, Pressure: %.2f hPa\n", data.temperature, data.pressure / 100.0f);
        printf("Altitude ft: %.2f\n", altitude_ft);
        sleep_ms(1'000);
    }
}
