#include <pico/stdio.h>

#include <Audible-Altimeter-Embedded/bmp_390.hpp>
#include <Audible-Altimeter-Embedded/timer.hpp>
#include <Audible-Altimeter-Embedded/tiny_2350.hpp>
#include <Audible-Altimeter/runner.hpp>

int main() {
  // Initialize stdio
  stdio_init_all();

  // Dependencies for embedded side.
  altimeter::Tiny2350 tiny;
  altimeter::BMP390 bmp390;
  altimeter::Timer timer;

  // Run the project
  altimeter::Runner runner(&tiny, &bmp390, &timer);
  runner.run();

  while (true) {
    tight_loop_contents();
  }
}