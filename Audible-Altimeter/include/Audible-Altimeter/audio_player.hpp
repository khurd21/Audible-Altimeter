#ifndef AUDIBLE_ALTIMETER_AUDIO_PLAYER_HPP
#define AUDIBLE_ALTIMETER_AUDIO_PLAYER_HPP
#include <Audible-Altimeter/sample_id.hpp>
#include <Audible-Altimeter/audio_driver.hpp>

#include <array>
#include <cstdint>

class Audio
{
public:
    Audio(AudioDriver & impl);

    bool play(audio_sample_id_t sample_id) const;

    void set_volume_on_all_samples(std::int16_t new_volume);

private:
    AudioDriver const& m_impl;
};
#endif // AUDIBLE_ALTIMETER_AUDIO_PLAYER_HPP