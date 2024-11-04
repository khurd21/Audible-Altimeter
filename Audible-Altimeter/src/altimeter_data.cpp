#include <Audible-Altimeter/altimeter_data.hpp>
#include <Audible-Altimeter/barometric_sensor_interface.hpp>
#include <cmath>
#include <concepts>
#include <optional>

namespace {

constexpr auto k_temperature_lapse_rate{0.0065};  // K/m
constexpr auto k_gas_constant{8.3144598};         // J / (mol * K)
constexpr auto k_gravity_acceleration{9.80665};   // m / s^2
constexpr auto k_molar_mass_air{0.0289644};       // kg / mol
constexpr auto k_celsius_to_kelvin{273.15};

// clang-format off
template <typename T>
requires std::is_arithmetic_v<T>
constexpr auto celsius_to_kelvin(T temperature_celsius) {
  return static_cast<T>(temperature_celsius + static_cast<T>(273.15));
}
// clang-format on

constexpr auto calculate_altitude_m(double base_temp_c, double base_pressure_pa,
                                    double pressure_pa) {
  const auto base_temp_k{celsius_to_kelvin(base_temp_c)};
  auto altitude_m = base_temp_k / k_temperature_lapse_rate;
  altitude_m *= 1 - std::pow(pressure_pa / base_pressure_pa, 0.1902612);
  return altitude_m;
}

}  // namespace

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
    result.altitude = calculate_altitude(data->pressure, data->temperature);
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

int AltimeterData::calculate_altitude(const double pressure_pa,
                                      const double temperature_c) const {
  if (!m_baseline_pressure || !m_baseline_temperature) {
    m_baseline_pressure = pressure_pa;
    m_baseline_temperature = temperature_c;
    return 0;
  }
  const auto altitude_m{calculate_altitude_m(
      *m_baseline_temperature, *m_baseline_pressure, pressure_pa)};
  if (m_measurement_system == MeasurementSystem::Imperial) {
    return static_cast<int>(altitude_m * 3.28084);
  }
  return static_cast<int>(altitude_m);
}

}  // namespace altimeter