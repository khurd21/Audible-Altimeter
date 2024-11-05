#ifndef AUDIBLE_ALTIMETER_AUDIO_PLAYER_HPP
#define AUDIBLE_ALTIMETER_AUDIO_PLAYER_HPP

#include <cstdint>

#include "sample_id.hpp"

namespace altimeter {

class IAudioDriver;

class AudioPlayer {
 public:
  AudioPlayer() = default;
  explicit AudioPlayer(IAudioDriver* audio_driver);

  bool play(AUDIO_SAMPLE_ID sample_id);

  void set_volume_on_all_samples(std::int16_t new_volume);

 private:
  IAudioDriver* m_audio_driver = nullptr;
};

}  // namespace altimeter

#endif  // AUDIBLE_ALTIMETER_AUDIO_PLAYER_HPP