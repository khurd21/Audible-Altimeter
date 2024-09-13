#include "pico/stdlib.h"
#include <cstdio>
#include "hello_world_48k_16bit_signed_mono.h"

const uint LED_PIN = PICO_DEFAULT_LED_PIN;
const int BLINK_DELAY_MS = 500;

void blink_led() {
    static bool led_state = false;
    gpio_put(LED_PIN, led_state);
    led_state = !led_state;
}

int main() {
    // Initialize stdio and GPIO
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Wait until serial connection is established
    // while (!stdio_usb_connected()) {
    //     blink_led(); // Blink LED while waiting for connection
    //     sleep_ms(BLINK_DELAY_MS);
    // }

    // Print all values in the array as hex
    for (std::size_t i = 0; i < hello_world_48k_16bit_signed_mono.size(); ++i) {
        printf("%04x ", static_cast<std::uint16_t>(hello_world_48k_16bit_signed_mono[i]));
        
        // New line every 16 values for readability
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }

        blink_led(); // Blink LED with every value printed
    }

    // Keep the LED blinking at a slower rate after printing
    while (true) {
        blink_led();
        sleep_ms(BLINK_DELAY_MS);
    }

    return 0;
}
