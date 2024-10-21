#ifndef MOCKS_DEVICE_DESCRIPTION_INTERFACE_HPP
#define MOCKS_DEVICE_DESCRIPTION_INTERFACE_HPP

#include <gmock/gmock.h>

#include <Audible-Altimeter/device_description_interface.hpp>

class MockDeviceDescription final : public altimeter::IDeviceDescription {
 public:
  MOCK_METHOD(void, toggle_led, (altimeter::IDeviceDescription::Color, bool),
              (override));

  MOCK_METHOD(void, toggle_leds, (bool), (override));
};

#endif  // MOCKS_DEVICE_DESCRIPTION_INTERFACE_HPP