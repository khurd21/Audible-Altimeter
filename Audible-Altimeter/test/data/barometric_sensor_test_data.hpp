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

using GatheredData = AltimeterData::GatheredData;
using SensorData = IBarometricSensor::SensorData;
using MeasurementSystem = AltimeterData::MeasurementSystem;

struct TemperatureValues {
  MeasurementSystem measurement_system;
  SensorData input_data;
  GatheredData expected_data;
};

struct PressureValues {
  MeasurementSystem measurement_system;
  std::vector<SensorData> input_data;
  std::vector<GatheredData> expected_data;
};

inline auto from_string(const std::string& string) {
  if (string == "Metric") {
    return MeasurementSystem::Metric;
  }
  if (string == "Imperial") {
    return MeasurementSystem::Imperial;
  }
  throw std::runtime_error("Invalid string for `MeasurementSystem.");
}

inline std::vector<TemperatureValues> get_temperature_test_data() {
  std::ifstream ifstream(std::string(k_data_folder) +
                         "temperature_test_data.json");

  if (!ifstream) {
    throw std::runtime_error("Unable to open test data json.");
  }
  std::vector<TemperatureValues> result;
  auto json{nlohmann::json::parse(ifstream)};
  if (json.is_array() && json.at(0).is_array()) {
    json = json.at(0);
  }

  for (const auto& item : json) {
    TemperatureValues temperature_value;

    // Measurement System
    temperature_value.measurement_system =
        from_string(item.at("measurement_system"));

    // Input Data
    temperature_value.input_data.pressure =
        item.at("input_data").at("pressure_pa");
    temperature_value.input_data.temperature =
        item.at("input_data").at("temperature_c");

    // Expected Data
    temperature_value.expected_data.measurement_system =
        temperature_value.measurement_system;
    temperature_value.expected_data.altitude =
        item.at("expected_data").at("altitude");
    temperature_value.expected_data.temperature =
        item.at("expected_data").at("temperature");

    result.emplace_back(std::move(temperature_value));
  }
  return result;
}

std::vector<PressureValues> get_pressure_test_data() {
  std::ifstream ifstream(std::string(k_data_folder) +
                         "pressure_test_data.json");
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
        from_string(item.at("measurement_system"));
    for (const auto& input_data : item.at("input_data")) {
      SensorData data;
      data.pressure = input_data.at("pressure_pa");
      data.temperature = input_data.at("temperature_c");
      pressure_value.input_data.emplace_back(std::move(data));
    }
    for (const auto& expected_data : item.at("expected_data")) {
      GatheredData data;
      data.measurement_system =
          from_string(expected_data.at("measurement_system"));
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