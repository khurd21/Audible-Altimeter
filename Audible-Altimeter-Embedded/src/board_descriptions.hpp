#ifndef AUDIBLE_ALTIMETER_EMBEDDED_BOARD_DESCRIPTIONS_HPP
#define AUDIBLE_ALTIMETER_EMBEDDED_BOARD_DESCRIPTIONS_HPP

#include <hardware/i2c.h>

#include <cstdint>

namespace altimeter::board_descriptions {

using I2CPort = i2c_inst_t*;
inline constexpr I2CPort I2C_PORT{i2c0};

inline constexpr std::uint32_t LED_PIN_RED{18};
inline constexpr std::uint32_t LED_PIN_GREEN{19};
inline constexpr std::uint32_t LED_PIN_BLUE{20};

inline constexpr std::uint32_t BMP390_SDA_PIN{12};
inline constexpr std::uint32_t BMP390_SCL_PIN{13};
inline constexpr std::uint32_t BMP390_ADDRESS{0x77};

}  // namespace altimeter::board_descriptions

#endif  // AUDIBLE_ALTIMETER_EMBEDDED_BOARD_DESCRIPTIONS_HPP
