#ifndef AUDIBLE_ALTIMETER_EMBEDDED_ITINY_2350_HPP
#define AUDIBLE_ALTIMETER_EMBEDDED_ITINY_2350_HPP

namespace altimeter {

/**
 * @brief
 * The primary interface for communicating with the Tiny2350 device.
 */
class ITiny2350 {
 public:
  ~ITiny2350() = default;

  /**
   * @brief
   * An enumeration representing each individual color supported by the tiny
   * 2350 device.
   */
  enum class Color {
    RED,
    BLUE,
    GREEN,
  };

  /**
   * @brief Toggles a single color on the device.
   * @param color The color for which to toggle.
   * @param on True for on, false for off.
   */
  virtual void toggle_led(Color color, bool on) const = 0;

  /**
   * @brief Toggles all of the colors on the device.
   * @param on True for on, false for off.
   */
  virtual void toggle_leds(bool on) const = 0;
};

}  // namespace altimeter

#endif  // AUDIBLE_ALTIMETER_EMBEDDED_ITINY_2350_HPP
