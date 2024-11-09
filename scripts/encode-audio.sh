#!/bin/bash

# Usage:
# ./conversion_script.sh <input_directory> <output_sample_rate>

input_dir="${1:-$(pwd)/assets/kyle_ai}"
output_sample_rate="${2:-8000}"
output_dir="$(pwd)/assets"

mkdir -p "$output_dir"
rm -f "$output_dir"/*.wav

for file in "$input_dir"/*.{mp3,wav}; do
  if [[ -f "$file" ]]; then
    filename=$(basename "$file" | sed 's/\.[^.]*$//')
    ffmpeg -i "$file" -ar "$output_sample_rate" -ac 1 -sample_fmt s16 "$output_dir/$filename.wav"
    echo "Converted $file to $output_dir/$filename.wav"
  fi
done