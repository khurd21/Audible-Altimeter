#ifndef _AUDIO_SAMPLES_H
#define _AUDIO_SAMPLES_H

#include "sample_id.h" // NUM_SAMPLES defined here

#include <cstdint>
#include <array>

struct Audio_sample_location_and_size {
    std::int16_t* location;
    std::size_t size;
};

extern std::array<std::int16_t, 11250> two;
extern std::array<std::int16_t, 12750> three;
extern std::array<std::int16_t, 13500> one;

using sample_lookup_t = std::array<Audio_sample_location_and_size,
                            AUDIO_SAMPLE_ID::NUM_SAMPLES>;

constexpr sample_lookup_t sample_lookup = {
        Audio_sample_location_and_size{ two.data(), two.size() },
        Audio_sample_location_and_size{ three.data(), three.size() },
        Audio_sample_location_and_size{ one.data(), one.size() },
    };
#endif // _AUDIO_SAMPLES_H
