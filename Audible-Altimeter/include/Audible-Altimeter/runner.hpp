#ifndef AUDIBLE_ALTIMETER_RUNNER_HPP
#define AUDIBLE_ALTIMETER_RUNNER_HPP

#include <Audible-Altimeter/barometric_sensor_interface.hpp>
#include <Audible-Altimeter/device_description_interface.hpp>
#include <Audible-Altimeter/timer_interface.hpp>

namespace altimeter {

/**
 * @brief
 * The primary runner for the Audible Altimeter. Collects all implemented
 * interfaces from the embedded library and utilizes them to build the program.
 */
class Runner {
 public:
  /**
   * @brief Constructs the Runner class.
   */
  Runner(IDeviceDescription* tiny_device, IBarometricSensor* barometric_sensor,
         ITimer* timer);

  /**
   * @brief Runs the Runner class.
   */
  void run();

 private:
  bool read_event();
  static bool read_event_callback(void* user_data);

  ITimer::CallbackData m_callback_data;
  IDeviceDescription* m_tiny_2350 = nullptr;
  IBarometricSensor* m_barometric_sensor = nullptr;
  ITimer* m_timer = nullptr;
};

}  // namespace altimeter

#endif  // AUDIBLE_ALTIMETER_RUNNER_HPP
