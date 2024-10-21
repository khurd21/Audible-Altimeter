#ifndef MOCKS_BAROMETRIC_SENSOR_INTERFACE
#define MOCKS_BAROMETRIC_SENSOR_INTERFACE

#include <gmock/gmock.h>

#include <Audible-Altimeter/barometric_sensor_interface.hpp>
#include <optional>

class MockBarometricSensor final : public altimeter::IBarometricSensor {
 public:
  MOCK_METHOD(std::optional<IBarometricSensor::SensorData>, get_sensor_data, (),
              (override));
};

#endif  // MOCKS_BAROMETRIC_SENSOR_INTERFACE