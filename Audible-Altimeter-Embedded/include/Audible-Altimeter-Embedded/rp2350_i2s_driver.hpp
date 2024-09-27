#ifndef RP2350_I2S_AUDIO_DRIVER
#define RP2350_I2S_AUDIO_DRIVER

#include <hardware/pio.h>
#include <stdio.h>

#include <Audible-Altimeter/audio_driver.hpp>
#include <cstddef>
#include <cstdint>

#include "hardware/dma.h"

/**
* @brief Manages the RP2350 specific hardware peripherals to play I2S audio
* @note audio must be in signed 16 bit mono pcm encoded 48kHz
* @details I2S is an audio standard for embedded devices that serializes
*          audio data into a bit stream. contineu.. TODO.
* @note This class only uses 1 state machine and 7 instructions of its pio
*       block.
*/
class RP2350I2SDriver final : public AudioDriver {
 public:
  /**
   * @brief Initializes hardware peripherals
   * @note i2s_clock_pin_base is the BIT_CLK i2s signal. LR_CLK
   *       must be physically connected the subsequent GPIO pin.
   *
   * @param pio_block the pio block for
   */
  RP2350I2SDriver(PIO pio_block, uint pio_sm, int dma_channel,
                  uint i2s_data_pin, uint i2s_clock_pin_base);

  bool play(std::int16_t* buffer, std::size_t buffer_length) override;
  bool is_playing() override;

 private:
  int m_dma_channel{0};
};

#endif  // RP2350_I2S_AUDIO_DRIVER