#include "hardware/dma.h"
#include "pico/binary_info.h"

#include <stdio.h>

#include "hardware/irq.h"
#include "audio_arrays/audio_samples.h"
#include "pio_serializer.pio.h"

// these are to convince the lsp that this symbol exists
// but then they complain abou how this will static assert false erggg
#ifndef PICO_AUDIO_I2S_CLOCK_PIN_BASE
#define PICO_AUDIO_I2S_CLOCK_PIN_BASE 1
// assert(false);
#endif
#ifndef PICO_AUDIO_I2S_DATA_PIN
#define PICO_AUDIO_I2S_DATA_PIN 1
// assert(false);
#endif

// these pin numbers are set in the cmake
bi_decl(bi_1pin_with_name(PICO_AUDIO_I2S_CLOCK_PIN_BASE,
                          "GPIO Pin # to 98357A BCLK"));
bi_decl(bi_1pin_with_name(PICO_AUDIO_I2S_CLOCK_PIN_BASE + 1,
                          "GPIO Pin # to 98357A LRC"));
bi_decl(bi_1pin_with_name(PICO_AUDIO_I2S_DATA_PIN, "GPIO Pin # to 98357A DIN"));

#define DMA_FINISHED_ISR_PIN 16

int dma_chan;

void dma_handler() {
    static bool even_odd{true};
    static uint counter { 0 };

    gpio_put(DMA_FINISHED_ISR_PIN, even_odd);


    // Clear the interrupt request.
    dma_hw->ints0 = 1u << dma_chan;

    dma_channel_set_trans_count(dma_chan, sample_lookup[counter].size, false);

    dma_channel_set_read_addr(dma_chan,
                              sample_lookup[counter].location, true);

    even_odd = !even_odd;
    counter+=1;
    if (counter == static_cast<uint>(AUDIO_SAMPLE::NUM_SAMPLES)) {
        counter = 0;
    }
}

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
    pio_serialiser_program_init(pio0, pio_state_machine, offset, PICO_AUDIO_I2S_DATA_PIN, PICO_AUDIO_I2S_CLOCK_PIN_BASE);

    dma_chan = dma_claim_unused_channel(true);
    dma_channel_config c = dma_channel_get_default_config(dma_chan);
    channel_config_set_transfer_data_size(&c, DMA_SIZE_16);

    channel_config_set_read_increment(&c, true);
    channel_config_set_write_increment(&c, false);
    channel_config_set_dreq(&c, DREQ_PIO0_TX0);

    dma_channel_configure(
        dma_chan, &c,
        &pio0_hw->txf[0],  // Write address (only need to set this once)
        audio_test_48k_16bit_mono
            .data(),  // Don't provide a read address yet
        audio_test_48k_16bit_mono.size(),
        false // don't start transfer
    );

    // Tell the DMA to raise IRQ line 0 when the channel finishes a block
    dma_channel_set_irq0_enabled(dma_chan, true);

    // Configure the processor to run dma_handler() when DMA IRQ 0 is asserted
    irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
    irq_set_enabled(DMA_IRQ_0, true);

    dma_channel_start(dma_chan);

    while (true) {
        tight_loop_contents();
    }
}
