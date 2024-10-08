#include <pico/stdlib.h>

#include <Audible-Altimeter-Embedded/tiny_2350.hpp>
#include <cstdint>
#include <cstdio>

#include "board_descriptions.hpp"

namespace {

auto is_initialized{false};

auto initialize() {
  if (is_initialized) {
    return;
  }

  printf("Initializing the Tiny2350. You should only see this message once.");
  gpio_init(altimeter::board_descriptions::LED_PIN_RED);
  gpio_init(altimeter::board_descriptions::LED_PIN_GREEN);
  gpio_init(altimeter::board_descriptions::LED_PIN_BLUE);

  gpio_set_dir(altimeter::board_descriptions::LED_PIN_RED, GPIO_OUT);
  gpio_set_dir(altimeter::board_descriptions::LED_PIN_GREEN, GPIO_OUT);
  gpio_set_dir(altimeter::board_descriptions::LED_PIN_BLUE, GPIO_OUT);

  is_initialized = true;
}

}  // namespace

namespace altimeter {

Tiny2350::Tiny2350() { initialize(); }

void Tiny2350::toggle_led(const Color color, const bool on) {
  switch (color) {
    case Color::BLUE:
      printf("Toggling LED Blue: %s", on ? "on" : "off");
      gpio_put(board_descriptions::LED_PIN_BLUE, !on);
      break;
    case Color::GREEN:
      printf("Toggling LED Green: %s", on ? "on" : "off");
      gpio_put(board_descriptions::LED_PIN_GREEN, !on);
      break;
    case Color::RED:
      printf("Toggling LED Red: %s", on ? "on" : "off");
      gpio_put(board_descriptions::LED_PIN_RED, !on);
      break;
  }
  printf("\n");
}

void Tiny2350::toggle_leds(const bool on) {
  toggle_led(Color::RED, on);
  toggle_led(Color::GREEN, on);
  toggle_led(Color::BLUE, on);
}

}  // namespace altimeter