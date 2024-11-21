#ifndef AUDIBLE_ALTIMETER_EMBEDDED_BOARD_DESCRIPTIONS_HPP
#define AUDIBLE_ALTIMETER_EMBEDDED_BOARD_DESCRIPTIONS_HPP

#include <hardware/i2c.h>

#include <cstdint>

#include "hardware/dma.h"
#include "hardware/pio.h"

#define AUDIO_PIO_BLOCK pio0
namespace altimeter::board_descriptions {

using I2CPort = i2c_inst_t*;
inline constexpr I2CPort I2C_PORT{i2c0};

inline constexpr std::uint8_t LED_PIN_RED{18};
inline constexpr std::uint8_t LED_PIN_GREEN{19};
inline constexpr std::uint8_t LED_PIN_BLUE{20};

inline constexpr std::uint8_t BMP390_SDA_PIN{12};
inline constexpr std::uint8_t BMP390_SCL_PIN{13};
inline constexpr std::uint8_t BMP390_ADDRESS{0x77};

inline constexpr std::uint8_t AUDIO_PIO_STATE_MACHINE{0};
inline constexpr std::uint8_t AUDIO_DMA_CHANNEL{0};

// BitCLK = 2, LRCLK = 3
inline constexpr std::uint32_t SPEAKER_I2S_CLK_BASE_PIN{2};
inline constexpr std::uint8_t SPEAKER_I2S_DATA_PIN{4};

}  // namespace altimeter::board_descriptions

#endif  // AUDIBLE_ALTIMETER_EMBEDDED_BOARD_DESCRIPTIONS_HPP
