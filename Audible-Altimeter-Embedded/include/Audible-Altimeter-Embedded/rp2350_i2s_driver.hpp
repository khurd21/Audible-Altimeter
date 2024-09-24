#ifndef RP2350_I2S_AUDIO_DRIVER
#define RP2350_I2S_AUDIO_DRIVER

#include <hardware/pio.h>
#include <stdio.h>

#include <Audible-Altimeter/audio_driver.hpp>
#include <cstddef>
#include <cstdint>

#include "hardware/dma.h"

class RP2350I2SDriver final : public AudioDriver {
 public:
  RP2350I2SDriver(uint i2s_data_pin, uint i2s_clock_pin_base);

  bool play(std::int16_t* buffer, std::size_t buffer_length) override;
  bool is_playing() override;

 private:
  PIO m_pio_block{pio0};
  uint m_pio_sm{0};
  uint m_pio_offset{0};

  int m_dma_chan{0};
  dma_channel_config m_dma_config;
};

#endif  // RP2350_I2S_AUDIO_DRIVER