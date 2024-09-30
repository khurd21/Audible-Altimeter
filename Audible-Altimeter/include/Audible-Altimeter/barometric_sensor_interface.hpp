#ifndef AUDIBLE_ALTIMETER_BAROMETRIC_SENSOR_INTERFACE_HPP
#define AUDIBLE_ALTIMETER_BAROMETRIC_SENSOR_INTERFACE_HPP

#include <optional>

namespace altimeter {

/**
 * @brief
 * Raw sensor data from the barometric sensor.
 *
 * - temperature: Temperature in celsius.
 *
 * - pressure: Pressure in Pascals.
 */
struct SensorData {
  double temperature{};
  double pressure{};
};

class IBarometricSensor {
 public:
  ~IBarometricSensor() = default;

  /**
   * @brief
   * Returns the sensor data, if available.
   * @returns The sensor data if available, otherwise std::nullopt.
   */
  virtual std::optional<SensorData> get_sensor_data() = 0;
};

}  // namespace altimeter

#endif  // AUDIBLE_ALTIMETER_BAROMETRIC_SENSOR_INTERFACE_HPP