#include <Audible-Altimeter/barometric_sensor_interface.hpp>
#include <Audible-Altimeter/device_description_interface.hpp>
#include <Audible-Altimeter/runner.hpp>
#include <cstdio>

namespace altimeter {

Runner::Runner(IDeviceDescription* tiny, IBarometricSensor* barometric_sensor) {
  m_tiny_2350 = tiny;
  m_barometric_sensor = barometric_sensor;
}

void Runner::run() {
  m_tiny_2350->toggle_leds(false);
  while (true) {
    const auto data{m_barometric_sensor->get_sensor_data()};
    if (data) {
      printf("Data T: %.2f deg C, P: %.2f Pa\n", data->temperature,
             data->pressure);
    }
  }
}

}  // namespace altimeter
