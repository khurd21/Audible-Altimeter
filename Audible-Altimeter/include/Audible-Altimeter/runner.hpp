#ifndef AUDIBLE_ALTIMETER_RUNNER_HPP
#define AUDIBLE_ALTIMETER_RUNNER_HPP

#include <Audible-Altimeter/altimeter_data.hpp>
#include <Audible-Altimeter/audio_driver_interface.hpp>
#include <Audible-Altimeter/audio_player.hpp>
#include <Audible-Altimeter/barometric_sensor_interface.hpp>
#include <Audible-Altimeter/device_description_interface.hpp>
#include <Audible-Altimeter/timer_interface.hpp>
#include <array>

#include "sample_id.hpp"

namespace altimeter {

/**
 * @brief
 * The primary runner for the Audible Altimeter. Collects all implemented
 * interfaces from the embedded library and utilizes them to build the program.
 */
class Runner {
 public:
  Runner() = default;

  /**
   * @brief Constructs the Runner class.
   */
  explicit Runner(IDeviceDescription* tiny_device,
                  IBarometricSensor* barometric_sensor, ITimer* timer,
                  IAudioDriver* audio_driver);

  /**
   * @brief Runs the Runner class.
   */
  void run();

 protected:
  bool read_event();

 private:
  enum class State {
    IDLE,
    ACTIVE,
  };

  void enter_state(State state);
  static bool read_event_callback(void* user_data);

  State m_state;
  int m_last_altitude_reading{};
  int m_index{};

  ITimer::CallbackData m_callback_data;
  AltimeterData m_altimeter_data;
  AudioPlayer m_audio_player;
  IDeviceDescription* m_tiny_2350 = nullptr;
  ITimer* m_timer = nullptr;
};

}  // namespace altimeter

#endif  // AUDIBLE_ALTIMETER_RUNNER_HPP
