#include <Audible-Altimeter/altimeter_data.hpp>
#include <Audible-Altimeter/barometric_sensor_interface.hpp>
#include <cmath>
#include <optional>

namespace altimeter {

AltimeterData::AltimeterData(IBarometricSensor* sensor,
                             MeasurementSystem system) {
  m_sensor = sensor;
  m_measurement_system = system;
  m_baseline_pressure = std::nullopt;
}

std::optional<AltimeterData::GatheredData> AltimeterData::get_data() const {
  AltimeterData::GatheredData result;
  const auto data{m_sensor->get_sensor_data()};
  if (data) {
    result.altitude = calculate_altitude(data->pressure);
    result.temperature = convert_temperature(data->temperature);
    result.measurement_system = m_measurement_system;
    return result;
  }
  return std::nullopt;
}

void AltimeterData::set_measurement_system(
    AltimeterData::MeasurementSystem system) {
  m_measurement_system = system;
}

int AltimeterData::convert_temperature(double temperature_c) const {
  if (m_measurement_system == AltimeterData::MeasurementSystem::Imperial) {
    return static_cast<int>(temperature_c * 9.0 / 5.0 + 32);
  }
  return static_cast<int>(temperature_c);
}

int AltimeterData::calculate_altitude(double pressure_pa) const {
  if (!m_baseline_pressure) {
    m_baseline_pressure = pressure_pa;
    return 0;
  }
  double altitude_m =
      (1.0 - std::pow(pressure_pa / m_baseline_pressure.value(), 0.1903)) *
      44330.0;
  if (m_measurement_system == MeasurementSystem::Imperial) {
    return static_cast<int>(altitude_m * 3.28084);  // Convert meters to feet
  }
  return static_cast<int>(altitude_m);
}

}  // namespace altimeter