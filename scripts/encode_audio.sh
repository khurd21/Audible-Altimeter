#!/bin/bash

OUTPUT_SAMPLE_RATE="8000"

# directions:
# make sure you run this script from the root directory e.g
# scripts/conversion_script.sh

input_dir="$(pwd)/assets/kyle_ai"
# input_dir="$(pwd)/assets/silas_recorded"
# input_dir="$(pwd)/assets/mp3"
output_dir="$(pwd)/assets"


# Ensure the output directory exists
mkdir -p "$output_dir"

# Remove all .wav files in the output directory
rm -f "$output_dir"/*.wav

# Loop through each mp3 file in the input directory
# for file in "$input_dir"/*.mp3; do
for file in "$input_dir"/two.wav  "$input_dir"/three.wav "$input_dir"/four.wav; do
# for file in "$input_dir"/two.mp3  "$input_dir"/three.mp3 "$input_dir"/four.mp3 ; do
  # Extract the base name of the file (without path and extension)
  filename=$(basename "$file" .wav)
  # filename=$(basename "$file" .mp3)

  # Convert mp3 to wav with specified settings
  ffmpeg -i "$file" -ar $OUTPUT_SAMPLE_RATE -ac 1 -sample_fmt s16 "$output_dir/$filename.wav"

  echo "Converted $file to $output_dir/$filename.wav"
done