#include <Audible-Altimeter/barometric_sensor_interface.hpp>
#include <Audible-Altimeter/device_description_interface.hpp>
#include <Audible-Altimeter/runner.hpp>
#include <Audible-Altimeter/timer_interface.hpp>
#include <array>
#include <chrono>
#include <cstdio>

namespace {

constexpr std::array<int, 32> k_altitudes_to_report{};
// 200 (two), 300 (three), 400 (four), 500 (five), 600 (six), 700 (seven), 800
// (eight), 900 (nine), 1000 (ten), 1100 (eleven), 1200 (twelve),
// 1300 (thirteen), 1400 (fourteen), 1500 (fifteen), 2000 (two), 2500 (two point
// five), 3000 (three), 3500 (three point five), 4000 (four), 4500 (four point
// five), 5000 (five), 5500 (five point five), 6000 (six), 7000 (seven), 8000
// (eight), 9000 (nine), 10'000 (ten), 11'000 (eleven), 12'000 (twelve), 13'000
// (thirteen), 14'000 (fourteen), 15'000 (fifteen)

constexpr auto k_one_thousand_index{8};

}  // namespace

namespace altimeter {

Runner::Runner(IDeviceDescription* tiny, IBarometricSensor* barometric_sensor,
               ITimer* timer) {
  m_tiny_2350 = tiny;
  m_altimeter_data = AltimeterData(barometric_sensor);
  m_timer = timer;
  m_callback_data.callback = &Runner::read_event_callback;
  m_callback_data.user_data = this;

  m_state = State::IDLE;
  m_last_altitude_reading = 0;
}

void Runner::run() {
  m_tiny_2350->toggle_leds(false);
  enter_state(State::IDLE);
};

bool Runner::read_event() {
  const auto data{m_altimeter_data.get_data()};
  if (!data) {
    return true;
  }
  const auto current_altitude{data->altitude};
  if (m_state == State::IDLE && current_altitude > 1'000) {
    enter_state(State::ACTIVE);
    m_index = k_one_thousand_index;
  } else if (m_state == State::ACTIVE) {
    if (current_altitude > k_altitudes_to_report[m_index + 1]) {
      ++m_index;
    } else if (current_altitude < k_altitudes_to_report[m_index]) {
      // report altitude at m_index;
      --m_index;
    }
    if (m_index == 0) {
      enter_state(State::IDLE);
    }
  }

  printf("Data T: %d degree F, P: %d ft\n", data->temperature, data->altitude);
  m_last_altitude_reading = current_altitude;
  return true;
}

void Runner::enter_state(Runner::State state) {
  m_state = state;
  switch (m_state) {
    case State::IDLE:
      m_timer->remove_repeating_timer();
      m_timer->add_repeating_timer(std::chrono::seconds(3), m_callback_data);
      break;
    case State::ACTIVE:
      m_timer->remove_repeating_timer();
      m_timer->add_repeating_timer(std::chrono::milliseconds(100),
                                   m_callback_data);
      break;
    default:
      printf("Unknown state entered: %d\n", static_cast<int>(m_state));
      printf("Returning to `ACTIVE` state in case user is skydiving.\n");
      enter_state(State::ACTIVE);
      break;
  }
}

bool Runner::read_event_callback(void* user_data) {
  auto data{static_cast<Runner*>(user_data)};
  if (data) {
    return data->read_event();
  }
  return false;
}

}  // namespace altimeter
