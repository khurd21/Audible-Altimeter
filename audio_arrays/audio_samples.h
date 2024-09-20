
#ifndef _AUDIO_SAMPLES_H
#define _AUDIO_SAMPLES_H

#include <cstdint>
#include <array>

enum class AUDIO_SAMPLE {
    TWO,
    THREE,
    AUDIO_TEST_48K_16BIT_MONO,
    ONE,
    NUM_SAMPLES,
};

struct Audio_sample_location_and_size {
    std::int16_t* location;
    std::size_t size;
};

extern std::array<std::int16_t, 11250> two;
extern std::array<std::int16_t, 12750> three;
extern std::array<std::int16_t, 120000> audio_test_48k_16bit_mono;
extern std::array<std::int16_t, 13500> one;

constexpr std::array<Audio_sample_location_and_size,
                     static_cast<std::size_t>(AUDIO_SAMPLE::NUM_SAMPLES)>
    sample_lookup = {
        Audio_sample_location_and_size{ two.data(), two.size() },
        Audio_sample_location_and_size{ three.data(), three.size() },
        Audio_sample_location_and_size{ audio_test_48k_16bit_mono.data(), audio_test_48k_16bit_mono.size() },
        Audio_sample_location_and_size{ one.data(), one.size() },
    };
#endif // _AUDIO_SAMPLES_H
