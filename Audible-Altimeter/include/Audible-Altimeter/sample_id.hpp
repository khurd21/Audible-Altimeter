#ifndef AUDIBLE_ALTIMETER_SAMPLE_ID_HPP
#define AUDIBLE_ALTIMETER_SAMPLE_ID_HPP

#include <cstdint>
#include <array>

using audio_sample_id_t = std::size_t;

namespace AUDIO_SAMPLE_ID {

constexpr audio_sample_id_t BEGIN_SAMPLES { 0 };
constexpr audio_sample_id_t TWO { 0 };
constexpr audio_sample_id_t THREE { 1 };
constexpr audio_sample_id_t ONE { 2 };
constexpr audio_sample_id_t NUM_SAMPLES { 3 };
constexpr audio_sample_id_t END_SAMPLES { 3 };

};

#endif // AUDIBLE_ALTIMETER_SAMPLE_ID_HPP