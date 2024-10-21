#ifndef AUDIBLE_ALTIMETER_ALTIMETER_DATA_HPP
#define AUDIBLE_ALTIMETER_ALTIMETER_DATA_HPP

#include <optional>

namespace altimeter {

class IBarometricSensor;

class AltimeterData {
 public:
  enum class MeasurementSystem {
    Imperial,
    Metric,
  };

  struct GatheredData {
    int altitude{};
    int temperature{};
    MeasurementSystem measurement_system{MeasurementSystem::Imperial};
  };

  AltimeterData() = default;
  explicit AltimeterData(
      IBarometricSensor* sensor,
      MeasurementSystem system = MeasurementSystem::Imperial);

  std::optional<GatheredData> get_data() const;
  void set_measurement_system(MeasurementSystem system);

 private:
  int convert_temperature(double temperature_c) const;
  int calculate_altitude(double pressure_pa, double temperature_c) const;

  IBarometricSensor* m_sensor = nullptr;
  MeasurementSystem m_measurement_system{MeasurementSystem::Imperial};
  mutable std::optional<double> m_baseline_pressure;
  mutable std::optional<double> m_baseline_temperature;
};

}  // namespace altimeter

#endif  // AUDIBLE_ALTIMETER_ALTIMETER_DATA_HPP