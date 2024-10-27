#ifndef AUDIBLE_ALTIMETER_BAROMETRIC_SENSOR_INTERFACE_HPP
#define AUDIBLE_ALTIMETER_BAROMETRIC_SENSOR_INTERFACE_HPP

#include <optional>

namespace altimeter {

class IBarometricSensor {
 public:
  ~IBarometricSensor() = default;

  /**
   * @brief
   * Raw sensor data from the barometric sensor.
   *
   * - temperature: Temperature in celsius.
   *
   * - pressure: Pressure in Pascals.
   */
  struct SensorData {
    SensorData() = default;
    explicit SensorData(double temperature, double pressure)
        : temperature(temperature), pressure(pressure) {}

    double temperature{};
    double pressure{};
  };

  /**
   * @brief
   * Returns the sensor data, if available.
   * @returns The sensor data if available, otherwise std::nullopt.
   */
  virtual std::optional<SensorData> get_sensor_data() = 0;
};

}  // namespace altimeter

#endif  // AUDIBLE_ALTIMETER_BAROMETRIC_SENSOR_INTERFACE_HPP