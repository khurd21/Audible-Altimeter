#include <hardware/dma.h>
#include <hardware/pio.h>
#include <pico/audio_i2s.h>

#include <Audible-Altimeter-Embedded/rp2350_i2s_driver.hpp>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>

#include "audio_samples.hpp"
#include "board_descriptions.hpp"
#include "i2s.pio.h"
#include "sample_id.hpp"

namespace {

audio_buffer_pool_t* k_audio_buffer_pool{};

}  // namespace

namespace altimeter {

RP2350I2SDriver::RP2350I2SDriver() {
  const audio_i2s_config_t config{
      .data_pin = board_descriptions::SPEAKER_I2S_CLK_BASE_PIN,
      .clock_pin_base = board_descriptions::SPEAKER_I2S_CLK_BASE_PIN,
      .dma_channel = board_descriptions::AUDIO_DMA_CHANNEL,
      .pio_sm = board_descriptions::AUDIO_PIO_STATE_MACHINE,
  };
  const audio_format_t format{
      .sample_freq = SAMPLE_RATE,
      .format = AUDIO_BUFFER_FORMAT_PCM_S16,
      .channel_count = 1,  // Mono
  };
  const auto applied_format{audio_i2s_setup(&format, &config)};
  if (!applied_format) {
    printf("Could not initialize I2S audio.\n");
  }

  if (!audio_i2s_connect(k_audio_buffer_pool)) {
    printf("Could not connect audio buffer pool to I2S.\n");
  }

  audio_i2s_set_enabled(true);
}

bool RP2350I2SDriver::play(const std::int16_t* sample,
                           std::size_t sample_size) {
  const auto buffer{take_audio_buffer(k_audio_buffer_pool, true)};
  const auto samples{(std::int16_t*)buffer->buffer->bytes};
  const auto volume_level{m_volume / 100.0f};

  for (std::uint32_t i{}; i < sample_size && i < buffer->max_sample_count;
       ++i) {
    samples[i] = static_cast<std::int16_t>(sample[i] * volume_level);
  }
  buffer->sample_count = std::min(static_cast<std::uint32_t>(sample_size),
                                  buffer->max_sample_count);
  give_audio_buffer(k_audio_buffer_pool, buffer);
  return true;
}

void RP2350I2SDriver::set_volume(std::uint8_t volume) {
  m_volume = std::min(volume, static_cast<std::uint8_t>(100));
  printf("Volume set to: %d percent.", m_volume);
}

bool RP2350I2SDriver::is_playing() { return false; }

}  // namespace altimeter