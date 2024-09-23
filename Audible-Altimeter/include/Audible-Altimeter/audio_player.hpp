#include "sample_id.h"
#include "samples/audio_samples.h"
#include <array>
#include <cstdint>

class Audio
{
public:
    Audio(AudioDriver const& impl);

    bool play(audio_sample_id_t sample_id) const;

    void set_volume_on_all_samples(std::int16_t new_volume);

private:
    AudioDriver const& m_impl;
};