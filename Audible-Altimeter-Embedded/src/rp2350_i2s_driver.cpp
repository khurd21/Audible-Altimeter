#include <stdio.h>

#include <Audible-Altimeter-Embedded/rp2350_i2s_driver.hpp>
#include <cstddef>
#include <cstdint>

#include "hardware/dma.h"
#include "hardware/pio.h"
#include "i2s.pio.h"

RP2350I2SDriver::RP2350I2SDriver(uint i2s_data_pin, uint i2s_clock_pin_base) {
  m_pio_offset = pio_add_program(pio0, &pio_i2s_program);
  pio_i2s_init(m_pio_block, m_pio_sm, m_pio_offset, i2s_data_pin,
               i2s_clock_pin_base);

  m_dma_chan = dma_claim_unused_channel(true);
  m_dma_config = dma_channel_get_default_config(m_dma_chan);
  channel_config_set_transfer_data_size(&m_dma_config, DMA_SIZE_16);

  channel_config_set_read_increment(&m_dma_config, true);
  channel_config_set_write_increment(&m_dma_config, false);

  // channel_config_set_dreq(&c, DREQ_PIO0_TX0);
  channel_config_set_dreq(&m_dma_config,
                          pio_get_dreq(m_pio_block, m_pio_sm, true));

  dma_channel_configure(m_dma_chan, &m_dma_config,
                        &pio0_hw->txf[0],  // Write address
                        nullptr,           // Don't provide a read address yet
                        0,                 // number bytes to transfer
                        false              // don't start transfer
  );
}

bool RP2350I2SDriver::play(std::int16_t* buffer, std::size_t buffer_length) {
  bool rval{true};
  if (!is_playing()) {
    dma_channel_set_trans_count(m_dma_chan, buffer_length, false);
    dma_channel_set_read_addr(m_dma_chan, buffer, true);
  } else {
    rval = false;
  }
  return rval;
}

bool RP2350I2SDriver::is_playing() { return dma_channel_is_busy(m_dma_chan); }