import wave
import struct

def wav_to_cpp_array(wav_file):
    with wave.open(wav_file, 'rb') as wav:
        # Ensure the WAV file is 16-bit signed integer with 48000 Hz sample rate and mono
        assert wav.getsampwidth() == 2, "WAV file is not 16-bit"
        assert wav.getframerate() == 48000, "WAV file is not 48000 Hz"
        assert wav.getnchannels() == 1, "WAV file is not mono"

        # Read frames and convert to 16-bit signed integers
        frames = wav.readframes(wav.getnframes())
        num_samples = wav.getnframes()
        
        # Unpack the frames into a list of 16-bit signed integers
        samples = struct.unpack(f'<{num_samples}h', frames)
        
        # Create the C++ array string
        cpp_array = '''
#include "hello_world_48k_16bit_signed_mono.h"
#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"\n'''
        cpp_array += f'int16_t hello_world_48k_16bit_signed_mono [{num_samples}] = {{\n'
        # cpp_array = f'std::array<std::int16_t, {num_samples}> data = {{\n'
        cpp_array += ',\n'.join(f'    {sample}' for sample in samples)
        cpp_array += '\n};'
        
        return cpp_array

# Usage
wav_file = '/Users/si/code/pico/vscode_picoexamples/pico-playground/assets/audio_test_48k_16bit_mono.wav'
cpp_array_str = wav_to_cpp_array(wav_file)

# Output the C++ array to a file or print
with open('hello_world_48k_16bit_signed_mono.c', 'w') as f:
    f.write(cpp_array_str)

# Print to console (optional)
print(cpp_array_str)
