#ifndef AUDIBLE_ALTIMETER_RUNNER_HPP
#define AUDIBLE_ALTIMETER_RUNNER_HPP

#include <Audible-Altimeter/altimeter_data.hpp>
#include <Audible-Altimeter/barometric_sensor_interface.hpp>
#include <Audible-Altimeter/device_description_interface.hpp>
#include <Audible-Altimeter/timer_interface.hpp>
#include <array>

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
  enum class State {
    IDLE,
    ACTIVE,
  };

  bool read_event();
  void enter_state(State state);
  static bool read_event_callback(void* user_data);

  State m_state;
  int m_last_altitude_reading{};
  int m_index{};

  ITimer::CallbackData m_callback_data;
  AltimeterData m_altimeter_data;
  IDeviceDescription* m_tiny_2350 = nullptr;
  ITimer* m_timer = nullptr;
};

}  // namespace altimeter

#endif  // AUDIBLE_ALTIMETER_RUNNER_HPP
