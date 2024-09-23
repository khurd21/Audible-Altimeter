#ifndef AUDIBLE_ALTIMETER_AUDIO_DRIVER_HPP
#define AUDIBLE_ALTIMETER_AUDIO_DRIVER_HPP
#include <cstdint>
#include <cstddef>

class AudioDriver
{
public:
    virtual bool play(std::int16_t* buffer, std::size_t buffer_length) = 0;

    virtual bool is_playing() = 0;
};
#endif // AUDIBLE_ALTIMETER_AUDIO_DRIVER_HPP