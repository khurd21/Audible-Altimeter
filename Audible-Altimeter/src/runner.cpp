#include <Audible-Altimeter/audio_driver_interface.hpp>
#include <Audible-Altimeter/audio_player.hpp>
#include <Audible-Altimeter/barometric_sensor_interface.hpp>
#include <Audible-Altimeter/device_description_interface.hpp>
#include <Audible-Altimeter/runner.hpp>
#include <Audible-Altimeter/timer_interface.hpp>
#include <array>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <limits>

namespace {

struct AltitudeToAudioSample {
  AltitudeToAudioSample() = default;
  constexpr explicit AltitudeToAudioSample(int altitude,
                                           AUDIO_SAMPLE_ID sample_id)
      : altitude(altitude), sample_id(sample_id) {}

  int altitude{};
  AUDIO_SAMPLE_ID sample_id{};
};

constexpr std::array<AltitudeToAudioSample, 8> k_altitudes{
    AltitudeToAudioSample(200, AUDIO_SAMPLE_ID::TWO),
    AltitudeToAudioSample(300, AUDIO_SAMPLE_ID::THREE),
    AltitudeToAudioSample(400, AUDIO_SAMPLE_ID::FOUR),
    AltitudeToAudioSample(1'000, AUDIO_SAMPLE_ID::ONE),
    AltitudeToAudioSample(2'000, AUDIO_SAMPLE_ID::TWO),
    AltitudeToAudioSample(3'000, AUDIO_SAMPLE_ID::THREE),
    AltitudeToAudioSample(4'000, AUDIO_SAMPLE_ID::FOUR),
    AltitudeToAudioSample(std::numeric_limits<int>::max(),
                          AUDIO_SAMPLE_ID::END_SAMPLES),
};

constexpr auto k_one_thousand_index{3};

}  // namespace

namespace altimeter {

Runner::Runner(IDeviceDescription* tiny, IBarometricSensor* barometric_sensor,
               ITimer* timer, IAudioDriver* audio_driver) {
  m_tiny_2350 = tiny;
  m_altimeter_data = AltimeterData(barometric_sensor);
  m_audio_player = AudioPlayer(audio_driver);
  m_timer = timer;
  m_callback_data.callback = &Runner::read_event_callback;
  m_callback_data.user_data = this;

  m_state = State::IDLE;
  m_last_altitude_reading = 0;

  m_tiny_2350->toggle_leds(false);
}

void Runner::run() { enter_state(State::IDLE); };

bool Runner::read_event() {
  const auto data{m_altimeter_data.get_data()};
  if (!data) {
    return true;
  }
  const auto current_altitude{data->altitude};
  if (m_state == State::IDLE && current_altitude > 1'000) {
    enter_state(State::ACTIVE);
    m_audio_player.play(k_altitudes[m_index].sample_id);
  } else if (m_state == State::ACTIVE) {
    const auto previous_index{m_index};
    while (current_altitude > k_altitudes[m_index + 1].altitude) {
      ++m_index;
    }
    if (current_altitude < k_altitudes[m_index].altitude) {
      m_audio_player.play(k_altitudes[m_index].sample_id);
      --m_index;
    }
    if (m_index < 0) {
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
      m_index = 0;
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
