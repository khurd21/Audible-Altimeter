
#include "hardware/dma.h"
#include "pio_serializer.pio.h"

#include <stdio.h>
#include <cstdint>
#include <cstddef>

RPI_I2S_Driver::RPI_I2S_Driver(uint i2s_data_pin, uint i2s_clock_pin_base) {
    m_pio_offset = pio_add_program(pio0, &pio_serialiser_program);
    pio_serialiser_program_init(m_pio_block, m_pio_sm, m_pio_offset, i2s_data_pin, i2s_clock_pin_base);

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

void RPI_I2S_Driver::play(std::int16_t* buffer, std::size_t buffer_length) {
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