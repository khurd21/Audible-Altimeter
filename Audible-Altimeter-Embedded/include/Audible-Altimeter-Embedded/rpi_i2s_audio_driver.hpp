#ifndef _RPI_I2S_AUDIO_DRIVER
#define _RPI_I2S_AUDIO_DRIVER

#include "audio_driver.h"

#include "hardware/dma.h"
#include "pio_serializer.pio.h"

#include <stdio.h>
#include <cstdint>
#include <cstddef>

class RPI_I2S_Driver final : public AudioDriver
{
public:
    RPI_I2S_Driver(uint i2s_data_pin, uint i2s_clock_pin_base);

    void play(std::int16_t* buffer, std::size_t buffer_length)  override;
    bool is_playing() override;

private:
    PIO m_pio_block { pio0 };
    uint m_pio_sm { 0 };
    uint m_pio_offset { 0 };

    int m_dma_chan { 0 };
    dma_channel_config m_dma_config;
}

#endif // _RPI_I2S_AUDIO_DRIVER