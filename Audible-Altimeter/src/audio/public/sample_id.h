#ifndef _SAMPLE_ID_H
#define _SAMPLE_ID_H

#include <cstdint>
#include <array>

using audio_sample_id_t = std::size_t;

namespace AUDIO_SAMPLE_ID {

constexpr audio_sample_id_t TWO { 0 };
constexpr audio_sample_id_t THREE { 1 };
constexpr audio_sample_id_t ONE { 2 };
constexpr audio_sample_id_t NUM_SAMPLES { 3 };

};

#endif // _SAMPLE_ID_H
