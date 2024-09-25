#include <Audible-Altimeter/device_description_interface.hpp>
#include <Audible-Altimeter/runner.hpp>

namespace altimeter {

Runner::Runner(IDeviceDescription* tiny) { m_tiny_2350 = tiny; }

void Runner::run() { m_tiny_2350->toggle_leds(false); }

}  // namespace altimeter
