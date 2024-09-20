#include "audio_player.h"
#include "sample_id.h"
#include "samples/audio_samples.h"
#include <array>
#include <cstdint>
#include <cstddef>

Audio::Audio(AudioDriver const& impl) : m_impl{impl}{}

bool Audio::play(audio_sample_id_t sample_id) const {
    assert(sample_id < AUDIO_SAMPLE_ID::NUM_SAMPLES);
    bool rval { false };
    if (sample_id < AUDIO_SAMPLE_ID::NUM_SAMPLES) {
        rval = m_impl.play(sample_lookup[sample_id].location, sample_lookup[sample_id].size);
    }
    return rval;
}

void Audio::set_volume_on_all_samples(std::int16_t new_volume) {
    for (audio_sample_id_t i { AUDIO_SAMPLE_ID::BEGIN_SAMPLES }; i < AUDIO_SAMPLE_ID::END_SAMPLES; ++i) {
        for (std::size_t j { 0 }; j < sample_lookup[i].size; ++j) {
            sample_lookup[i].location[j] = (sample_lookup[i].location[j] * new_volume) >> 8u;
        }
    }
}