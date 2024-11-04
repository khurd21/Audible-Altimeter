#include <pico/stdio.h>
#include <pico/stdlib.h>

#include <Audible-Altimeter-Embedded/bmp_390.hpp>
#include <Audible-Altimeter-Embedded/rp2350_i2s_driver.hpp>
#include <Audible-Altimeter-Embedded/timer.hpp>
#include <Audible-Altimeter-Embedded/tiny_2350.hpp>
#include <Audible-Altimeter/audio_player.hpp>
#include <Audible-Altimeter/runner.hpp>
#include <Audible-Altimeter/timer_interface.hpp>

int main() {
  stdio_init_all();

  // Dependencies for embedded side.
  altimeter::Tiny2350 tiny;
  altimeter::BMP390 bmp390;
  altimeter::Timer timer;
  altimeter::RP2350I2SDriver rp2350_audio_driver;
  altimeter::AudioPlayer audio_player(&rp2350_audio_driver);

  altimeter::Runner runner(&tiny, &bmp390, &timer, &rp2350_audio_driver);
  runner.run();

  while (true) {
    tight_loop_contents();
  }
}