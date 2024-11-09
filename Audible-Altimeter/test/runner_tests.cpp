#include <gtest/gtest.h>

#include <Audible-Altimeter/audio_driver_interface.hpp>
#include <Audible-Altimeter/barometric_sensor_interface.hpp>
#include <Audible-Altimeter/device_description_interface.hpp>
#include <Audible-Altimeter/runner.hpp>
#include <Audible-Altimeter/timer_interface.hpp>
#include <chrono>
#include <optional>
#include <utility>
#include <vector>

#include "data/runner_test_data.hpp"
#include "mocks/audio_driver_interface.hpp"
#include "mocks/barometric_sensor_interface.hpp"
#include "mocks/device_description_interface.hpp"
#include "mocks/timer_interface.hpp"

namespace {

using testing::_;
using testing::Return;
using testing::Test;
using testing::ValuesIn;
using testing::WithParamInterface;

class RunnerSubClass : public altimeter::Runner {
 public:
  using Runner::read_event;
  using Runner::Runner;
};

}  // namespace

namespace altimeter::tests {

class RunnerTestsBase : public Test {
 protected:
  RunnerTestsBase() {
    EXPECT_CALL(m_mock_device_description, toggle_leds(false)).Times(1);
    m_runner_under_test =
        RunnerSubClass(&m_mock_device_description, &m_mock_barometric_sensor,
                       &m_mock_timer, &m_mock_audio_driver);
  }

  RunnerSubClass m_runner_under_test;

  MockTimer m_mock_timer;
  MockAudioDriver m_mock_audio_driver;
  MockBarometricSensor m_mock_barometric_sensor;
  MockDeviceDescription m_mock_device_description;
};

class RunnerTests : public RunnerTestsBase {};

class RunnerParamTests
    : public RunnerTestsBase,
      public WithParamInterface<std::vector<RunnerTestValues>> {};

TEST_F(RunnerTests, RunBeginsInIDLEState) {
  constexpr auto expected_ms{
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::seconds(3))};

  EXPECT_CALL(m_mock_timer, remove_repeating_timer).Times(1);
  EXPECT_CALL(m_mock_timer, add_repeating_timer(expected_ms, _));
  m_runner_under_test.run();
}

TEST_F(RunnerTests, ReadEventDoesNothingWhenReceivingNullData) {
  EXPECT_CALL(m_mock_barometric_sensor, get_sensor_data)
      .WillOnce(Return(std::nullopt));
  EXPECT_TRUE(m_runner_under_test.read_event());
}

TEST_P(RunnerParamTests, MockedSkydive) {
  const auto& param{GetParam()};
  for (const auto& value : param) {
    EXPECT_CALL(m_mock_barometric_sensor, get_sensor_data)
        .WillOnce(Return(IBarometricSensor::SensorData(value.temperature_c,
                                                       value.pressure_pa)));
    if (value.sample_id) {
      EXPECT_CALL(m_mock_audio_driver, play(_, _)).WillOnce(Return(true));
    }
    if (value.entered_active_state || value.entered_idle_state) {
      EXPECT_CALL(m_mock_timer, remove_repeating_timer).Times(1);
      EXPECT_CALL(m_mock_timer, add_repeating_timer(_, _)).Times(1);
    }
    std::ignore = m_runner_under_test.read_event();
  }
}

INSTANTIATE_TEST_SUITE_P(RunnerParamTestSuite, RunnerParamTests,
                         ValuesIn(get_runner_test_data()));

}  // namespace altimeter::tests