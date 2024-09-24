#include <Audible-Altimeter-Embedded/Tiny2350.hpp>
#include <cstdint>

#include "pico/stdlib.h"

namespace {

constexpr std::uint32_t LED_PIN_RED{18};
constexpr std::uint32_t LED_PIN_GREEN{19};
constexpr std::uint32_t LED_PIN_BLUE{20};

auto is_initialized{false};

auto initialize() {
  if (is_initialized) {
    return;
  }

  gpio_init(LED_PIN_RED);
  gpio_init(LED_PIN_GREEN);
  gpio_init(LED_PIN_BLUE);

  gpio_set_dir(LED_PIN_RED, GPIO_OUT);
  gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
  gpio_set_dir(LED_PIN_BLUE, GPIO_OUT);

  is_initialized = true;
}

}  // namespace

namespace altimeter {

Tiny2350::Tiny2350() { initialize(); }

void Tiny2350::toggle_led(const Color color, const bool on) const {
  switch (color) {
    case Color::BLUE:
      gpio_put(LED_PIN_BLUE, !on);
      break;
    case Color::GREEN:
      gpio_put(LED_PIN_GREEN, !on);
      break;
    case Color::RED:
      gpio_put(LED_PIN_RED, !on);
      break;
  }
}

void Tiny2350::toggle_leds(const bool on) const {
  toggle_led(Color::RED, on);
  toggle_led(Color::GREEN, on);
  toggle_led(Color::BLUE, on);
}

}  // namespace altimeter