


# i2s notes:

When LRCLK has a 50% duty cycle the data
format is determined by the part number selection
(MAX98357A/MAX98357B). When a frame sync pulse
is used for the LRCLK the data format is automatically
configured in TDM mode.

# BCLK Polarity pg. 16
Mode I2S BCLK Polarity: rising edge

# LRCLK
- left channel is low


# standby mode power consumption:
- standby mode has reduced power consumption from normal operation (340µA),
- Drive SD_MODE low to put the ICs into shutdown, shutodwn takes 0.6uA of power


# pico audio lib link:
https://github.com/raspberrypi/pico-extras/blob/master/src/common/pico_audio/include/pico/audio.
https://github.com/malacalypse/rp2040_i2s_example
https://github.com/raspberrypi/FreeRTOS-Kernel

# sd pin:
This pin is used for shutdown mode but is also used for setting which channel is output. It's a little confusing but essentially:

If SD is connected to ground directly (voltage is under 0.16V) then the amp is shut down
If the voltage on SD is between 0.16V and 0.77V then the output is (Left + Right)/2, that is the stereo average.
If the voltage on SD is between 0.77V and 1.4V then the output is just the Right channel
If the voltage on SD is higher than 1.4V then the output is the Left channel.
This is compounded by an internal 100K pulldown resistor on SD so you need to use a pullup resistor on SD to balance out the 100K internal pulldown.

For the breakout board, there's a 1Mohm resistor from SD to Vin which, when powering from 5V will give you the 'stereo average' output. If you want left or right channel only, or if you are powering from non-5V power, you may need to experiment with different resistors to get the desired voltage on SD


# sdk notes:

- uf2 is just used to allow flashing over boot drive
- elf executable linkable format
- can use cmake variable PICO_PLATFORM to set the enfironment we're building for, including host, which provides interfaces to pico sdk library. check pg 24 of pico sdk, directory structure.
-

You can take as much control of this process as you want, but by default, the pico_runtime includes these libraries: • pico_crt0 - the runtime entry point and default linker scripts which define memory layout
• pico_standard_link - configuration for link options and pulling in linker scripts
• pico_runtime_init - a default set of initializers to run before reaching main.


There are a handful of other C++ related PICO_CXX vars listed in Chapter 6.


audio export options:
48k, mono, normalize off, sample rate 48k, pcm encoed, wav, 16 bit, triangular dither