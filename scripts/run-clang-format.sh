#!/bin/bash

# Find all .cpp and .hpp files excluding the build directory and run clang-format on each
find . \( -name '*.cpp' -o -name '*.hpp' \) -not -path "./build/*" | while read file; do
    echo "Formatting $file"
    clang-format -i "$file"
done

echo "Clang-format applied to all .cpp and .hpp files, excluding the build directory."
