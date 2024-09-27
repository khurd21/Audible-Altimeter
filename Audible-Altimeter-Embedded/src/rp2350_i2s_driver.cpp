#include <stdio.h>

#include <Audible-Altimeter-Embedded/rp2350_i2s_driver.hpp>
#include <cstddef>
#include <cstdint>

#include "board_descriptions.hpp"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "i2s.pio.h"
RP2350I2SDriver::RP2350I2SDriver() : m_dma_channel{AUDIO_DMA_CHANNEL} {
  uint pio_offset = pio_add_program(AUDIO_PIO_BLOCK, &pio_i2s_program);

  pio_i2s_init(AUDIO_PIO_BLOCK, AUDIO_PIO_STATE_MACHINE, pio_offset,
               altimeter::board_descriptions::LED_PIN_I2S_DATA_PIN,
               altimeter::board_descriptions::LED_PIN_I2S_CLK_BASE);

  dma_channel_config dma_config = dma_channel_get_default_config(m_dma_channel);

  channel_config_set_transfer_data_size(&dma_config, DMA_SIZE_16);

  constexpr bool INCREMENT_READ_ADDR{true};
  channel_config_set_read_increment(&dma_config, INCREMENT_READ_ADDR);

  constexpr bool DONT_INCREMENT_WRITE_ADDR{false};
  channel_config_set_write_increment(&dma_config, DONT_INCREMENT_WRITE_ADDR);

  channel_config_set_dreq(
      &dma_config,
      pio_get_dreq(AUDIO_PIO_BLOCK, AUDIO_PIO_STATE_MACHINE, true));

  constexpr bool DONT_START_TRANSFER{false};
  constexpr uint TRANSFER_COUNT{0};
  constexpr void* READ_ADDR{nullptr};
  dma_channel_configure(m_dma_channel, &dma_config,
                        &((AUDIO_PIO_BLOCK)->txf[0]),  // Write address
                        READ_ADDR,           // Don't provide a read address yet
                        TRANSFER_COUNT,      // number bytes to transfer
                        DONT_START_TRANSFER  // don't start transfer
  );
}

RP2350I2SDriver::RP2350I2SDriver(PIO pio_block, uint pio_sm, int dma_channel,
                                 uint i2s_data_pin, uint i2s_clock_pin_base)
    : m_dma_channel{dma_channel} {
  uint pio_offset = pio_add_program(pio_block, &pio_i2s_program);

  pio_i2s_init(pio_block, pio_sm, pio_offset, i2s_data_pin, i2s_clock_pin_base);

  dma_channel_config dma_config = dma_channel_get_default_config(m_dma_channel);

  channel_config_set_transfer_data_size(&dma_config, DMA_SIZE_16);

  constexpr bool INCREMENT_READ_ADDR{true};
  channel_config_set_read_increment(&dma_config, INCREMENT_READ_ADDR);

  constexpr bool DONT_INCREMENT_WRITE_ADDR{false};
  channel_config_set_write_increment(&dma_config, DONT_INCREMENT_WRITE_ADDR);

  channel_config_set_dreq(&dma_config, pio_get_dreq(pio_block, pio_sm, true));

  constexpr bool DONT_START_TRANSFER{false};
  constexpr uint TRANSFER_COUNT{0};
  constexpr void* READ_ADDR{nullptr};
  dma_channel_configure(m_dma_channel, &dma_config,
                        &pio_block->txf[0],  // Write address
                        READ_ADDR,           // Don't provide a read address yet
                        TRANSFER_COUNT,      // number bytes to transfer
                        DONT_START_TRANSFER  // don't start transfer
  );
}

bool RP2350I2SDriver::play(std::int16_t* buffer, std::size_t buffer_length) {
  bool rval{true};
  if (!is_playing()) {
    constexpr bool DONT_START{false};
    dma_channel_set_trans_count(m_dma_channel, buffer_length, DONT_START);

    constexpr bool START{true};
    dma_channel_set_read_addr(m_dma_channel, buffer, START);
  } else {
    rval = false;
  }
  return rval;
}

bool RP2350I2SDriver::is_playing() {
  return dma_channel_is_busy(m_dma_channel);
}