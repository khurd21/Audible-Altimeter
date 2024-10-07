#include <Audible-Altimeter/barometric_sensor_interface.hpp>
#include <Audible-Altimeter/device_description_interface.hpp>
#include <Audible-Altimeter/runner.hpp>
#include <Audible-Altimeter/timer_interface.hpp>
#include <cstdio>

namespace altimeter {

Runner::Runner(IDeviceDescription* tiny, IBarometricSensor* barometric_sensor,
               ITimer* timer) {
  m_tiny_2350 = tiny;
  m_barometric_sensor = barometric_sensor;
  m_timer = timer;
  m_callback_data.callback = &Runner::read_event_callback;
  m_callback_data.user_data = this;
}

void Runner::run() {
  m_tiny_2350->toggle_leds(false);
  m_timer->add_repeating_timer(std::chrono::milliseconds(500), m_callback_data);
};

bool Runner::read_event() {
  const auto data{m_barometric_sensor->get_sensor_data()};
  if (data) {
    printf("Data T: %.2f deg C, P: %.2f Pa\n", data->temperature,
           data->pressure);
  }
  return true;
}

bool Runner::read_event_callback(void* user_data) {
  auto data{static_cast<Runner*>(user_data)};
  if (data) {
    return data->read_event();
  }
  return false;
}

}  // namespace altimeter
