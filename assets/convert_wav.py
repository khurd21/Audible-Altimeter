import os
import wave
import struct

AUDIO_FILE_DIRECTORY = "./"
AUDIO_ARRAY_DIRECTORY = "../audio_arrays"
header_file_name = "audio_samples"

class SampleMetadata:
    full_path: str
    filename: str
    sample_name: str
    samples: list[int]
    num_samples: int


def get_samples(wav_file):
    samples = []
    with wave.open(wav_file, 'rb') as wav:
        assert wav.getsampwidth() == 2, "WAV file is not 16-bit"
        assert wav.getframerate() == 48000, "WAV file is not 48000 Hz"
        assert wav.getnchannels() == 1, "WAV file is not mono"

        # Read frames and convert to 16-bit signed integers
        num_samples = wav.getnframes()
        frames = wav.readframes(num_samples)

        # Unpack the frames into a list of 16-bit signed integers
        samples = struct.unpack(f'<{num_samples}h', frames)
    return samples

def create_header_file(samples: list[SampleMetadata]):
    header_file_path = os.path.abspath(os.path.join(AUDIO_ARRAY_DIRECTORY, f'{header_file_name}.h'))
    header_file_contents = f'''
#ifndef _{header_file_name.upper()}_H
#define _{header_file_name.upper()}_H

#include <cstdint>
#include <array>\n
'''
    header_file_contents += 'enum class AUDIO_SAMPLE {\n'

    for sample in samples:
        header_file_contents += f'    {sample.sample_name.upper()},\n'
    header_file_contents += "    NUM_SAMPLES,\n"
    header_file_contents += '};\n'
    header_file_contents += '''
struct Audio_sample_location_and_size {
    std::int16_t* location;
    std::size_t size;
};\n
'''
    for sample in samples:
        header_file_contents += f'extern std::array<std::int16_t, {sample.num_samples}> {sample.sample_name};\n'

    header_file_contents += '''
constexpr std::array<Audio_sample_location_and_size,
                     static_cast<std::size_t>(AUDIO_SAMPLE::NUM_SAMPLES)>
    sample_lookup = {
'''
    for sample in samples:
        header_file_contents += f'        Audio_sample_location_and_size{{ {sample.sample_name}.data(), {sample.sample_name}.size() }},\n'
    header_file_contents += "    };"

    header_file_contents += f'''
#endif // _{header_file_name.upper()}_H
'''
    with open(header_file_path, 'w') as f:
        f.write(header_file_contents)  # Write each sample name on a new line


def create_cpp_files(samples: list[SampleMetadata]):
    # for sample in samples:
    for sample in samples:
        cpp_file_path = os.path.abspath(os.path.join(AUDIO_ARRAY_DIRECTORY, f'{sample.sample_name}.cpp'))

        # Create the C++ array string
        cpp_file_contents = f'''#include "{header_file_name}.h"

#include <array>
#include <cstdint>\n
'''
        cpp_file_contents += f'std::array<std::int16_t, {sample.num_samples}> {sample.sample_name} = {{\n'
        cpp_file_contents += ',\n'.join(f'    {num}' for num in sample.samples)
        cpp_file_contents += '\n};'
        with open(cpp_file_path, 'w') as f:
            f.write(cpp_file_contents)  # Write each sample name on a new line
def create_cmake_file(samples: list[SampleMetadata]):
    cmake_file_path = os.path.abspath(os.path.join(AUDIO_ARRAY_DIRECTORY, 'CMakeLists.txt'))

    # Collect source file names
    cpp_files = [f'{sample.sample_name}.cpp' for sample in samples]

    cmake_file_contents = f'''# Auto-generated CMakeLists.txt for audio samples
set(SOURCES
    {header_file_name}.h
'''
    cmake_file_contents += ''.join(f'    {cpp_file}\n' for cpp_file in cpp_files)
    cmake_file_contents += ''')

# add_library(audio_samples STATIC ${SOURCE_FILES})

# Specify include directories
target_include_directories(audio_samples PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
'''
    with open(cmake_file_path, 'w') as f:
        f.write(cmake_file_contents)

def collect_wav_samples_files(directory):
    sample_list: list[SampleMetadata] = []

    wav_files = []
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith(".wav"):
                wav_files.append(os.path.abspath(os.path.join(root, file)))

    for wav in wav_files:
        data = SampleMetadata()
        data.full_path = wav
        data.sample_name = os.path.splitext(os.path.basename(wav))[0]
        data.samples = get_samples(wav)
        data.num_samples = len(data.samples)
        sample_list.append(data)

    return sample_list


def main():
    samples = collect_wav_samples_files(AUDIO_FILE_DIRECTORY)
    create_header_file(samples)
    create_cpp_files(samples)
    # create_cmake_file(samples)

if __name__ == '__main__':
    main()
