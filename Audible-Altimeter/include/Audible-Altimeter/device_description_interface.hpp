#ifndef AUDIBLE_ALTIMETER_DEVICE_DESCRIPTION_INTERFACE_HPP
#define AUDIBLE_ALTIMETER_DEVICE_DESCRIPTION_INTERFACE_HPP

namespace altimeter {

class IDeviceDescription {
 public:
  ~IDeviceDescription() = default;

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
  virtual void toggle_led(Color color, bool on) = 0;

  /**
   * @brief Toggles all of the colors on the device.
   * @param on True for on, false for off.
   */
  virtual void toggle_leds(bool on) = 0;
};

}  // namespace altimeter

#endif  // AUDIBLE_ALTIMETER_DEVICE_DESCRIPTION_INTERFACE_HPP
