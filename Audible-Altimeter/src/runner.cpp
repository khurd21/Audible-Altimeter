#include <Audible-Altimeter/audio_driver_interface.hpp>
#include <Audible-Altimeter/audio_player.hpp>
#include <Audible-Altimeter/barometric_sensor_interface.hpp>
#include <Audible-Altimeter/device_description_interface.hpp>
#include <Audible-Altimeter/runner.hpp>
#include <Audible-Altimeter/timer_interface.hpp>
#include <array>
#include <chrono>
#include <cstdio>
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

constexpr std::array<AltitudeToAudioSample, 29> k_altitudes{
    AltitudeToAudioSample(200, AUDIO_SAMPLE_ID::TWO),
    AltitudeToAudioSample(300, AUDIO_SAMPLE_ID::THREE),
    AltitudeToAudioSample(400, AUDIO_SAMPLE_ID::FOUR),
    // AltitudeToAudioSample(500, AUDIO_SAMPLE_ID::FIVE),
    // AltitudeToAudioSample(600, AUDIO_SAMPLE_ID::SIX),
    // AltitudeToAudioSample(700, AUDIO_SAMPLE_ID::SEVEN),
    // AltitudeToAudioSample(800, AUDIO_SAMPLE_ID::EIGHT),
    // AltitudeToAudioSample(900, AUDIO_SAMPLE_ID::NINE),
    // AltitudeToAudioSample(1'000, AUDIO_SAMPLE_ID::TEN),
    // AltitudeToAudioSample(1'500, AUDIO_SAMPLE_ID::FIFTEEN),
    // AltitudeToAudioSample(2'000, AUDIO_SAMPLE_ID::TWO),
    // AltitudeToAudioSample(2'500, AUDIO_SAMPLE_ID::TWO_POINT_FIVE),
    // AltitudeToAudioSample(3'000, AUDIO_SAMPLE_ID::THREE),
    // AltitudeToAudioSample(3'500, AUDIO_SAMPLE_ID::THREE_POINT_FIVE),
    // AltitudeToAudioSample(4'000, AUDIO_SAMPLE_ID::FOUR),
    // AltitudeToAudioSample(4'500, AUDIO_SAMPLE_ID::FOUR_POINT_FIVE),
    // AltitudeToAudioSample(5'000, AUDIO_SAMPLE_ID::FIVE),
    // AltitudeToAudioSample(5'500, AUDIO_SAMPLE_ID::FIVE_POINT_FIVE),
    // AltitudeToAudioSample(6'000, AUDIO_SAMPLE_ID::SIX),
    // AltitudeToAudioSample(7'000, AUDIO_SAMPLE_ID::SEVEN),
    // AltitudeToAudioSample(8'000, AUDIO_SAMPLE_ID::EIGHT),
    // AltitudeToAudioSample(9'000, AUDIO_SAMPLE_ID::NINE),
    // AltitudeToAudioSample(10'000, AUDIO_SAMPLE_ID::TEN),
    // AltitudeToAudioSample(11'000, AUDIO_SAMPLE_ID::ELEVEN),
    // AltitudeToAudioSample(12'000, AUDIO_SAMPLE_ID::TWELVE),
    // AltitudeToAudioSample(13'000, AUDIO_SAMPLE_ID::THIRTEEN),
    // AltitudeToAudioSample(14'000, AUDIO_SAMPLE_ID::FOURTEEN),
    // AltitudeToAudioSample(15'000, AUDIO_SAMPLE_ID::FIFTEEN),
    // AltitudeToAudioSample(std::numeric_limits<int>::max(),
    //                       AUDIO_SAMPLE_ID::END_SAMPLES),
};

constexpr auto one_thousand_index{8};

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
  static std::size_t i{0};
  const auto data{m_altimeter_data.get_data()};
  if (!data) {
    return true;
  }
  const auto current_altitude{data->altitude};
  if (m_state == State::IDLE && current_altitude > 1'000) {
    enter_state(State::ACTIVE);
    m_audio_player.play(AUDIO_SAMPLE_ID::TEN);
    m_index = one_thousand_index;
  } else if (m_state == State::ACTIVE) {
    if (current_altitude > k_altitudes[m_index + 1].altitude) {
      ++m_index;
    } else if (current_altitude < k_altitudes[m_index].altitude) {
      m_audio_player.play(k_altitudes[m_index].sample_id);
      --m_index;
    }
    if (m_index < 0) {
      enter_state(State::IDLE);
    }
  }
  m_audio_player.play(k_altitudes[i++].sample_id);
  if (i == k_altitudes.size() - 1) i = 0;

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
