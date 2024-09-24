#include <Audible-Altimeter/audio_player.hpp>
#include <Audible-Altimeter/sample_id.hpp>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>

#include "audio_samples.hpp"

AudioPlayer::AudioPlayer(AudioDriver& impl) : m_impl{impl} {}

bool AudioPlayer::play(AUDIO_SAMPLE_ID sample_id) {
  bool rval{false};
  auto _sample_id{static_cast<std::uint32_t>(sample_id)};
  if (_sample_id < static_cast<std::uint32_t>(AUDIO_SAMPLE_ID::NUM_SAMPLES)) {
    rval = m_impl.play(sample_lookup[_sample_id].location,
                       sample_lookup[_sample_id].size);
  } else {
    assert(false);
  }
  return rval;
}

void AudioPlayer::set_volume_on_all_samples(std::int16_t new_volume) {
  // for (AUDIO_SAMPLE_ID i { AUDIO_SAMPLE_ID::BEGIN_SAMPLES }; i <
  // AUDIO_SAMPLE_ID::END_SAMPLES; i =
  // static_cast<AUDIO_SAMPLE_ID>(static_cast<std::uint32_t>(i) + 1)) {
  //     for (std::size_t j { 0 }; j < sample_lookup[i].size; ++j) {
  //         sample_lookup[i].location[j] = (sample_lookup[i].location[j] *
  //         new_volume) >> 8u;
  //     }
  // }
}