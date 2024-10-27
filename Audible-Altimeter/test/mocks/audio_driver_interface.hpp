#ifndef MOCKS_AUDIO_DRIVER_INTERFACE_HPP
#define MOCKS_AUDIO_DRIVER_INTERFACE_HPP

#include <gmock/gmock.h>

#include <Audible-Altimeter/audio_driver_interface.hpp>
#include <cstdint>

class MockAudioDriver final : public altimeter::IAudioDriver {
 public:
  MOCK_METHOD(bool, play, (std::int16_t*, std::size_t), (override));
  MOCK_METHOD(bool, is_playing, (), (override));
};

#endif  // MOCKS_AUDIO_DRIVER_INTERFACE_HPP