#include <pico/stdio.h>

#include <Audible-Altimeter-Embedded/tiny_2350.hpp>
#include <Audible-Altimeter/runner.hpp>

int main() {
  // Initialize stdio
  stdio_init_all();

  // Dependencies for embedded side.
  altimeter::Tiny2350 tiny;

  // Run the project
  altimeter::Runner runner(&tiny);
  runner.run();
}