#ifndef AUDIBLE_ALTIMETER_AUDIO_DRIVER_HPP
#define AUDIBLE_ALTIMETER_AUDIO_DRIVER_HPP
#include <cstddef>
#include <cstdint>

namespace altimeter {

class IAudioDriver {
 public:
  ~IAudioDriver() = default;

  virtual bool play(std::int16_t* buffer, std::size_t buffer_length) = 0;

  virtual bool is_playing() = 0;
};

}  // namespace altimeter

#endif  // AUDIBLE_ALTIMETER_AUDIO_DRIVER_HPP