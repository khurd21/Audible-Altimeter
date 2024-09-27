#include <Audible-Altimeter/audio_player.hpp>
#include <Audible-Altimeter/device_description_interface.hpp>
#include <Audible-Altimeter/runner.hpp>
#include <Audible-Altimeter/sample_id.hpp>

// #include "pico/stdlib.h"

namespace altimeter {

#define LED_DELAY_MS 500

Runner::Runner(IDeviceDescription* tiny) { m_tiny_2350 = tiny; }

void Runner::run() { m_tiny_2350->toggle_leds(false); }

}  // namespace altimeter
