#ifndef RP2350_I2S_AUDIO_DRIVER
#define RP2350_I2S_AUDIO_DRIVER

#include <hardware/pio.h>
#include <stdio.h>

#include <Audible-Altimeter/audio_driver_interface.hpp>
#include <cstddef>
#include <cstdint>

#include "hardware/dma.h"

namespace altimeter {

/**
 * @brief Manages the RP2350 specific hardware peripherals to play I2S audio
 * @note audio must be in signed 16 bit mono pcm encoded 48kHz
 * @details I2S is an audio standard for embedded devices that serializes
 *          audio data into a bit stream. contineu.. TODO.
 * @note This class only uses 1 state machine and 7 instructions of its pio
 *       block.
 */
class RP2350I2SDriver final : public IAudioDriver {
 public:
  /**
   * @brief Initializes hardware peripherals
   * @note i2s_clock_pin_base is the BIT_CLK i2s signal. LR_CLK
   *       must be physically connected the subsequent GPIO pin.
   *
   * @param pio_block the pio block for
   */
  RP2350I2SDriver();

  bool play(const std::int16_t* buffer, std::size_t buffer_length) override;
  void set_volume(std::uint8_t volume) override;
  bool is_playing() override;

 private:
  std::uint8_t m_volume{};
};

}  // namespace altimeter

#endif  // RP2350_I2S_AUDIO_DRIVER