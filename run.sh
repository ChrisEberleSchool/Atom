#!/bin/bash

# Navigate to the build directory
BUILD_DIR="$(dirname "$0")/build"

if [ ! -d "$BUILD_DIR" ]; then
    echo "Build directory not found, creating..."
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
    cmake ..
else
    cd "$BUILD_DIR"
fi

# Build the project
echo "Building project..."
cmake --build .

# Check if build succeeded
if [ $? -eq 0 ]; then
    echo "Build succeeded. Launching program..."
    ./atom
else
    echo "Build failed. Fix errors first."
fi
