#include <gtest/gtest.h>

#include <Audible-Altimeter/altimeter_data.hpp>
#include <Audible-Altimeter/barometric_sensor_interface.hpp>
#include <vector>

#include "data/barometric_sensor_test_data.hpp"
#include "mocks/barometric_sensor_interface.hpp"

namespace {

using testing::Return;
using testing::Sequence;
using testing::Test;
using testing::TestWithParam;
using testing::ValuesIn;

}  // namespace

namespace altimeter::tests {

template <typename T>
class AltimeterDataBaseTests : public TestWithParam<T> {
 protected:
  AltimeterDataBaseTests() {
    m_altimeter_data_under_test = AltimeterData(&m_mock_barometric_sensor);
  }

  void set_measurement_system(MeasurementSystem system) {
    m_altimeter_data_under_test =
        AltimeterData(&m_mock_barometric_sensor, system);
  }

  AltimeterData m_altimeter_data_under_test;
  MockBarometricSensor m_mock_barometric_sensor;
};

class AltimeterDataTemperatureTests
    : public AltimeterDataBaseTests<TemperatureValues> {};

TEST_P(AltimeterDataTemperatureTests, Temp) {
  const auto& params{GetParam()};
  set_measurement_system(params.measurement_system);
  EXPECT_CALL(m_mock_barometric_sensor, get_sensor_data)
      .WillOnce(Return(IBarometricSensor::SensorData(
          params.input_data.temperature, params.input_data.pressure)));

  const auto result{m_altimeter_data_under_test.get_data()};

  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->temperature, params.expected_data.temperature);
}

INSTANTIATE_TEST_SUITE_P(TemperatureTests, AltimeterDataTemperatureTests,
                         ValuesIn(get_temperature_test_data()));

class AltimeterDataPressureTests
    : public AltimeterDataBaseTests<PressureValues> {};

TEST_P(AltimeterDataPressureTests, Pressure) {
  const auto& params{GetParam()};
  ASSERT_FALSE(params.expected_data.empty());
  set_measurement_system(params.measurement_system);
  ASSERT_EQ(params.input_data.size(), params.expected_data.size());
  Sequence s;
  for (const auto& param : params.input_data) {
    EXPECT_CALL(m_mock_barometric_sensor, get_sensor_data)
        .InSequence(s)
        .WillOnce(Return(
            IBarometricSensor::SensorData(param.temperature, param.pressure)));
  }

  for (const auto& param : params.expected_data) {
    const auto result{m_altimeter_data_under_test.get_data()};
    EXPECT_EQ(result->altitude, param.altitude);
  }
}

INSTANTIATE_TEST_SUITE_P(PressureTests, AltimeterDataPressureTests,
                         ValuesIn(get_pressure_test_data()));

}  // namespace altimeter::tests