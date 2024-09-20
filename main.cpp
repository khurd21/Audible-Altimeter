#include "hardware/dma.h"

#include <stdio.h>

#include "hardware/irq.h"
#include "audio_arrays/audio_samples.h"
#include "pio_serializer.pio.h"

// PIO sends one bit per 10 system clock cycles. DMA sends the same 32-bit
// value 10 000 times before halting. This means we cycle through the 32 PWM
// levels roughly once per second.
#define PIO_SERIAL_CLKDIV 10.f
#define PWM_REPEAT_COUNT 10000
#define N_PWM_LEVELS 32

#define DMA_FINISHED_ISR_PIN 16

int dma_chan;

void dma_handler() {
    static int pwm_level = 0;
    static uint32_t wavetable[N_PWM_LEVELS];
    static bool first_run = true;
    static bool even_odd{true};
    static uint counter { 0 };

    gpio_put(DMA_FINISHED_ISR_PIN, even_odd);


    // Clear the interrupt request.
    dma_hw->ints0 = 1u << dma_chan;

    dma_channel_set_trans_count(dma_chan, sample_lookup[counter].size, false);

    // dma_channel_set_trans_count(
    //     dma_chan, audio_test_48k_16bit_mono.size(), false);
    // dma_channel_set_read_addr(dma_chan,
    //                           audio_test_48k_16bit_mono.data(), true);

    dma_channel_set_read_addr(dma_chan,
                              sample_lookup[counter].location, true);

    even_odd = !even_odd;
    counter+=1;
    if (counter == static_cast<uint>(AUDIO_SAMPLE::NUM_SAMPLES)) {
        counter = 0;
    }
}

// unsigned int data_array[] = { 1, 2, 3, 4, 5, 6, 7, 8};

int main() {
    gpio_init(DMA_FINISHED_ISR_PIN);
    gpio_set_dir(DMA_FINISHED_ISR_PIN, GPIO_OUT);

    constexpr std::int16_t volume { 100 };
    for (auto& sample : audio_test_48k_16bit_mono)
    {
        sample = (sample * volume) >> 8U;
    }

    // Set up a PIO state machine to serialise our bits
    uint offset = pio_add_program(pio0, &pio_serialiser_program);

    uint pio_state_machine{0};
    pio_serialiser_program_init(pio0, pio_state_machine, offset, PICO_AUDIO_I2S_DATA_PIN, PICO_AUDIO_I2S_CLOCK_PIN_BASE,
                                PIO_SERIAL_CLKDIV);

    // Configure a channel to write the same word (32 bits) repeatedly to PIO0
    // SM0's TX FIFO, paced by the data request signal from that peripheral.
    dma_chan = dma_claim_unused_channel(true);
    dma_channel_config c = dma_channel_get_default_config(dma_chan);
    channel_config_set_transfer_data_size(&c, DMA_SIZE_16);
    // channel_config_set_transfer_data_size(&c, DMA_SIZE_32);
    // channel_config_set_read_increment(&c, false);
    channel_config_set_read_increment(&c, true);
    channel_config_set_write_increment(&c, false);
    channel_config_set_dreq(&c, DREQ_PIO0_TX0);

    dma_channel_configure(
        dma_chan, &c,
        &pio0_hw->txf[0],  // Write address (only need to set this once)
        audio_test_48k_16bit_mono
            .data(),  // Don't provide a read address yet
        // PWM_REPEAT_COUNT, // Write the same value many times, then halt and
        // interrupt
        audio_test_48k_16bit_mono.size(),
        // 100,
        false  // Don't start yet
    );

    // Tell the DMA to raise IRQ line 0 when the channel finishes a block
    dma_channel_set_irq0_enabled(dma_chan, true);

    // Configure the processor to run dma_handler() when DMA IRQ 0 is asserted
    irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
    irq_set_enabled(DMA_IRQ_0, true);

    dma_channel_start(dma_chan);

    // Manually call the handler once, to trigger the first transfer
    // dma_handler();

    // Everything else from this point is interrupt-driven. The processor has
    // time to sit and think about its early retirement -- maybe open a bakery?
    while (true) {
        tight_loop_contents();
    }
}

// #include <stdio.h>
// #include "pico/stdlib.h"
// #include "hardware/dma.h"

// // Data will be copied from src to dst
// const char src[] = "Hello, world! (from DMA)";
// char dst[count_of(src)];

// int main() {
//     stdio_init_all();

//     // Get a free channel, panic() if there are none
//     int chan = dma_claim_unused_channel(true);

//     // 8 bit transfers. Both read and write address increment after each
//     // transfer (each pointing to a location in src or dst respectively).
//     // No DREQ is selected, so the DMA transfers as fast as it can.

//     dma_channel_config c = dma_channel_get_default_config(chan);
//     channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
//     channel_config_set_read_increment(&c, true);
//     channel_config_set_write_increment(&c, true);

//     dma_channel_configure(
//         chan,          // Channel to be configured
//         &c,            // The configuration we just created
//         dst,           // The initial write address
//         src,           // The initial read address
//         count_of(src), // Number of transfers; in this case each is 1 byte.
//         true           // Start immediately.
//     );

//     // We could choose to go and do something else whilst the DMA is doing
//     its
//     // thing. In this case the processor has nothing else to do, so we just
//     // wait for the DMA to finish.
//     dma_channel_wait_for_finish_blocking(chan);

//     // The DMA has now copied our text from the transmit buffer (src) to the
//     // receive buffer (dst), so we can print it out from there.
//     puts(dst);
// }
