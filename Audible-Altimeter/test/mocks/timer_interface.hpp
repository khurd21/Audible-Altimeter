#ifndef MOCKS_TIMER_INTERFACE_HPP
#define MOCKS_TIMER_INTERFACE_HPP

#include <gmock/gmock.h>

#include <Audible-Altimeter/timer_interface.hpp>
#include <chrono>

class MockTimer final : public altimeter::ITimer {
 public:
  MOCK_METHOD(void, add_repeating_timer,
              (std::chrono::milliseconds, altimeter::ITimer::CallbackData&),
              (override));
  MOCK_METHOD(void, remove_repeating_timer, (), (override));
};

#endif  // MOCKS_TIMER_INTERFACE_HPP