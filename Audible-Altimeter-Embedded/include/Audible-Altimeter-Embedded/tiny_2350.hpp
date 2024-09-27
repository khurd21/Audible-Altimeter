#ifndef AUDIBLE_ALTIMETER_EMBEDDED_TINY_2350_HPP
#define AUDIBLE_ALTIMETER_EMBEDDED_TINY_2350_HPP

#include <Audible-Altimeter/device_description_interface.hpp>

namespace altimeter {

class Tiny2350 : public IDeviceDescription {
 public:
  Tiny2350();

  void toggle_led(Color color, bool on) override;
  void toggle_leds(bool on) override;
};

}  // namespace altimeter

#endif  // AUDIBLE_ALTIMETER_EMBEDDED_TINY_2350_HPP