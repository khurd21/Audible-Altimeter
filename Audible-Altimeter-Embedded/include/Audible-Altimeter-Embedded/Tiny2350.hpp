#ifndef AUDIBLE_ALTIMETER_EMBEDDED_TINY_2350_HPP
#define AUDIBLE_ALTIMETER_EMBEDDED_TINY_2350_HPP

#include <Audible-Altimeter-Embedded/ITiny2350.hpp>

namespace altimeter {

class Tiny2350 : public ITiny2350 {
 public:
  Tiny2350();

  void toggle_led(Color color, bool on) const override;
  void toggle_leds(bool on) const override;
};

}  // namespace altimeter

#endif  // AUDIBLE_ALTIMETER_EMBEDDED_TINY_2350_HPP