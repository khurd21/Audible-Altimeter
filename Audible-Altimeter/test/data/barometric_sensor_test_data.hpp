#ifndef TEST_DATA_ALTIMETER_TEST_DATA_HPP
#define TEST_DATA_ALTIMETER_TEST_DATA_HPP

#include <Audible-Altimeter/altimeter_data.hpp>
#include <Audible-Altimeter/barometric_sensor_interface.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "path_to_data_folder.hpp"

namespace altimeter::tests {
namespace detail {

inline auto from_string(const std::string& string) {
  if (string == "Metric") {
    return AltimeterData::MeasurementSystem::Metric;
  }
  if (string == "Imperial") {
    return AltimeterData::MeasurementSystem::Imperial;
  }
  throw std::runtime_error("Invalid string for `MeasurementSystem.");
}

}  // namespace detail

struct TemperatureValues {
  AltimeterData::MeasurementSystem measurement_system;
  IBarometricSensor::SensorData input_data;
  AltimeterData::GatheredData expected_data;
};

struct PressureValues {
  AltimeterData::MeasurementSystem measurement_system;
  std::vector<IBarometricSensor::SensorData> input_data;
  std::vector<AltimeterData::GatheredData> expected_data;
};

inline std::vector<TemperatureValues> get_temperature_test_data() {
  std::ifstream ifstream(k_data_folder +
                         std::string("/temperature_test_data.json"));

  if (!ifstream) {
    throw std::runtime_error("Unable to open test data json.");
  }
  std::vector<TemperatureValues> result;
  auto json{nlohmann::json::parse(ifstream)};
  if (json.is_array() && json.at(0).is_array()) {
    json = json.at(0);
  }

  for (const auto& item : json) {
    IBarometricSensor::SensorData input_data(
        item.at("input_data").at("temperature_c"),
        item.at("input_data").at("pressure_pa"));

    AltimeterData::GatheredData expected_data{
        .altitude = item.at("expected_data").at("altitude"),
        .temperature = item.at("expected_data").at("temperature"),
        .measurement_system =
            detail::from_string(item.at("measurement_system"))};

    TemperatureValues temperature_value{
        .measurement_system =
            detail::from_string(item.at("measurement_system")),
        .input_data = input_data,
        .expected_data = expected_data};

    result.emplace_back(temperature_value);
  }
  return result;
}

std::vector<PressureValues> get_pressure_test_data() {
  std::ifstream ifstream(k_data_folder +
                         std::string("/pressure_test_data.json"));
  if (!ifstream) {
    throw std::runtime_error("Could not parse pressure test data json.");
  }
  std::vector<PressureValues> result;
  auto json{nlohmann::json::parse(ifstream)};
  if (json.is_array() && json.at(0).is_array()) {
    json = json.at(0);
  }

  for (const auto& item : json) {
    PressureValues pressure_value;
    pressure_value.measurement_system =
        detail::from_string(item.at("measurement_system"));
    for (const auto& input_data : item.at("input_data")) {
      IBarometricSensor::SensorData data;
      data.pressure = input_data.at("pressure_pa");
      data.temperature = input_data.at("temperature_c");
      pressure_value.input_data.emplace_back(std::move(data));
    }
    for (const auto& expected_data : item.at("expected_data")) {
      AltimeterData::GatheredData data;
      data.measurement_system =
          detail::from_string(expected_data.at("measurement_system"));
      data.altitude = expected_data.at("altitude");
      data.temperature = expected_data.at("temperature");
      pressure_value.expected_data.emplace_back(std::move(data));
    }
    result.emplace_back(std::move(pressure_value));
  }
  return result;
}

}  // namespace altimeter::tests

#endif  // TEST_DATA_ALTIMETER_TEST_DATA_HPP