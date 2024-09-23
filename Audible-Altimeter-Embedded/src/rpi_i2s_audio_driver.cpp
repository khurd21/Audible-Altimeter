#include <Audible-Altimeter-Embedded/rpi_i2s_audio_driver.hpp>
#include "i2s.pio.h"

#include "hardware/dma.h"
#include "hardware/pio.h"

#include <stdio.h>
#include <cstdint>
#include <cstddef>

// int abc { 123 };

// #define pio_i2s_wrap_target 0
// #define pio_i2s_wrap 7
// #define pio_i2s_pio_version 1

// #define pio_i2s_offset_entry_point 7u

// static const uint16_t pio_i2s_program_instructions[] = {
//             //     .wrap_target
//     0x7001, //  0: out    pins, 1         side 2
//     0x1840, //  1: jmp    x--, 0          side 3
//     0x6001, //  2: out    pins, 1         side 0
//     0xe82e, //  3: set    x, 14           side 1
//     0x6001, //  4: out    pins, 1         side 0
//     0x0844, //  5: jmp    x--, 4          side 1
//     0x7001, //  6: out    pins, 1         side 2
//     0xf82e, //  7: set    x, 14           side 3
//             //     .wrap
// };

// #if !PICO_NO_HARDWARE
// static const struct pio_program pio_i2s_program = {
//     .instructions = pio_i2s_program_instructions,
//     .length = 8,
//     .origin = -1,
//     .pio_version = 1,
// #if PICO_PIO_VERSION > 0
//     .used_gpio_ranges = 0x0
// #endif
// };

// static inline pio_sm_config pio_i2s_program_get_default_config(uint offset) {
//     pio_sm_config c = pio_get_default_sm_config();
//     sm_config_set_wrap(&c, offset + pio_i2s_wrap_target, offset + pio_i2s_wrap);
//     sm_config_set_sideset(&c, 2, false, false);
//     return c;
// }

// clk_pin_base is bitclk then lrc
// #include "pico/time.h"
// #include "hardware/clocks.h"
// static inline void pio_i2s_init(PIO pio, uint sm, uint offset, uint data_pin, uint clk_pin_base) {
//     pio_gpio_init(pio, data_pin);
//     pio_gpio_init(pio, clk_pin_base);
//     pio_gpio_init(pio, clk_pin_base + 1);
//     // pio_sm_set_consecutive_pindirs(pio, sm, clk_pin_base, 3, true);
//     // pio_sm_set_consecutive_pindirs(pio, sm, data_pin, 1, true);
//     pio_sm_config c = pio_i2s_program_get_default_config(offset);
//     uint pin_mask = (0b01 << data_pin) | (0b11 << clk_pin_base); // 0b11 << clock_pin_base
//     pio_sm_set_pindirs_with_mask(pio, sm, pin_mask, pin_mask);
//     sm_config_set_out_pins(&c, data_pin, 1);
//     sm_config_set_sideset_pins(&c, clk_pin_base);
//     // sm_config_set_out_shift(&c, true, true, 32);
//     sm_config_set_out_shift(&c, false, true, 32);
//     sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);
//     uint sample_rate = 48000;
//     uint f_clk_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS);
//     sm_config_set_clkdiv(&c, (f_clk_sys * 1000.f) / (sample_rate * 32 * 2.0f));  // 32 bits * 2 (clock signal goes high/low every bit)
//     pio_sm_init(pio, sm, offset, &c);
//     pio_sm_set_enabled(pio, sm, true);
// }
// #endif


RPI_I2S_Driver::RPI_I2S_Driver(uint i2s_data_pin, uint i2s_clock_pin_base) {
    m_pio_offset = pio_add_program(pio0, &pio_i2s_program);
    pio_i2s_init(m_pio_block, m_pio_sm, m_pio_offset, i2s_data_pin, i2s_clock_pin_base);

    m_dma_chan = dma_claim_unused_channel(true);
    m_dma_config = dma_channel_get_default_config(m_dma_chan);
    channel_config_set_transfer_data_size(&m_dma_config, DMA_SIZE_16);

    channel_config_set_read_increment(&m_dma_config, true);
    channel_config_set_write_increment(&m_dma_config, false);

    // channel_config_set_dreq(&c, DREQ_PIO0_TX0);
    channel_config_set_dreq(&m_dma_config, pio_get_dreq(m_pio_block, m_pio_sm, true));

    dma_channel_configure(
        m_dma_chan, &m_dma_config,
        &pio0_hw->txf[0],  // Write address
        nullptr,  // Don't provide a read address yet
        0, // number bytes to transfer
        false // don't start transfer
    );
}

bool RPI_I2S_Driver::play(std::int16_t* buffer, std::size_t buffer_length) {
    bool rval { true };
    if (!is_playing()) {
        dma_channel_set_trans_count(m_dma_chan, buffer_length, false);
        dma_channel_set_read_addr(m_dma_chan, buffer, true);
    } else {
        rval = false;
    }
    return rval;
}

bool RPI_I2S_Driver::is_playing() {
    return dma_channel_is_busy(m_dma_chan);
}