#include <Audible-Altimeter-Embedded/ITiny2350.hpp>
#include <Audible-Altimeter/Runner.hpp>

namespace altimeter {

Runner::Runner(ITiny2350* tiny) { m_tiny_2350 = tiny; }

void Runner::run() { m_tiny_2350->toggle_leds(false); }

}  // namespace altimeter
