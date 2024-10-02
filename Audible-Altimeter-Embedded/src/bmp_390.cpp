#include <BMP3_SensorAPI/bmp3.h>
#include <BMP3_SensorAPI/bmp3_defs.h>
#include <hardware/gpio.h>
#include <hardware/i2c.h>
#include <pico/stdlib.h>

#include <Audible-Altimeter-Embedded/bmp_390.hpp>
#include <cstdio>
#include <optional>

#include "bmp_390_communications.hpp"
#include "board_descriptions.hpp"

namespace {}

namespace altimeter {

BMP390::BMP390() {
  bmp3_check_rslt("bmp3_interface_init",
                  bmp3_interface_init(&m_bmp3, BMP3_I2C_INTF));
  bmp3_check_rslt("bmp3_init", bmp3_init(&m_bmp3));

  m_settings.int_settings.drdy_en = BMP3_ENABLE;
  m_settings.press_en = BMP3_ENABLE;
  m_settings.temp_en = BMP3_ENABLE;

  m_settings.odr_filter.press_os = BMP3_OVERSAMPLING_2X;
  m_settings.odr_filter.temp_os = BMP3_OVERSAMPLING_2X;
  m_settings.odr_filter.odr = BMP3_ODR_100_HZ;

  const std::uint16_t settings_sel = BMP3_SEL_PRESS_EN | BMP3_SEL_TEMP_EN |
                                     BMP3_SEL_PRESS_OS | BMP3_SEL_TEMP_OS |
                                     BMP3_SEL_ODR | BMP3_SEL_DRDY_EN;
  bmp3_check_rslt("bmp3_set_sensor_settings",
                  bmp3_set_sensor_settings(settings_sel, &m_settings, &m_bmp3));

  m_settings.op_mode = BMP3_MODE_NORMAL;
  bmp3_check_rslt("bmp3_set_op_mode", bmp3_set_op_mode(&m_settings, &m_bmp3));
}

std::optional<SensorData> BMP390::get_sensor_data() {
  bmp3_status status;
  const auto result{bmp3_get_status(&status, &m_bmp3)};
  bmp3_check_rslt("bmp3_get_status", result);
  if (result == BMP3_OK && status.intr.drdy == BMP3_ENABLE) {
    bmp3_data data;
    bmp3_check_rslt("bmp3_get_sensor_data",
                    bmp3_get_sensor_data(BMP3_PRESS_TEMP, &data, &m_bmp3));

    // NOTE: Read status register again to clear data ready interrupt status
    bmp3_check_rslt("bmp3_get_status", bmp3_get_status(&status, &m_bmp3));
    return SensorData{data.temperature, data.pressure};
  }
  //
  return std::nullopt;
}

}  // namespace altimeter