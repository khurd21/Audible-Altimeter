#include <pico/stdio.h>

#include <Audible-Altimeter-Embedded/bmp_390.hpp>
#include <Audible-Altimeter-Embedded/rp2350_i2s_driver.hpp>
#include <Audible-Altimeter-Embedded/timer.hpp>
#include <Audible-Altimeter-Embedded/tiny_2350.hpp>
#include <Audible-Altimeter/audio_player.hpp>
#include <Audible-Altimeter/runner.hpp>
#include <Audible-Altimeter/sample_id.hpp>

#include "pico/stdlib.h"
#define LED_DELAY_MS 1000

int main() {
  // Initialize stdio
  stdio_init_all();

  // Dependencies for embedded side.
  altimeter::Tiny2350 tiny;
  altimeter::BMP390 bmp390;
  altimeter::Timer timer;

  RP2350I2SDriver rp2350_audio_driver;
  AudioPlayer audio_player{rp2350_audio_driver};

  uint audio_id{static_cast<uint>(AUDIO_SAMPLE_ID::BEGIN_SAMPLES)};

  while (true) {
    sleep_ms(LED_DELAY_MS);
    audio_player.play(static_cast<AUDIO_SAMPLE_ID>(audio_id));

    ++audio_id;
    if (audio_id == static_cast<uint>(AUDIO_SAMPLE_ID::END_SAMPLES)) {
      audio_id = static_cast<uint>(AUDIO_SAMPLE_ID::BEGIN_SAMPLES);
    }
  }
  // Run the project
  altimeter::Runner runner(&tiny, &bmp390, &timer);
  runner.run();

  while (true) {
    tight_loop_contents();
  }
}