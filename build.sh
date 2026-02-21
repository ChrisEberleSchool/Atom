#!/bin/bash

# Ensure we are in the project root (where this script is)
PROJECT_ROOT="$(cd "$(dirname "$0")" && pwd)"
cd "$PROJECT_ROOT"

# Build directory
BUILD_DIR="$PROJECT_ROOT/build"

# Remove old build directory
if [ -d "$BUILD_DIR" ]; then
    echo "Removing old build directory..."
    rm -rf "$BUILD_DIR"
fi

# Recreate build directory
mkdir "$BUILD_DIR"
cd "$BUILD_DIR"

# Run CMake configuration from project root
echo "Configuring project with CMake..."
cmake ..

# Build the project
echo "Building project..."
cmake --build .

# Check if build succeeded
if [ $? -eq 0 ]; then
    echo "Build succeeded. Launching program..."
    ./atom
else
    echo "Build failed. Fix errors first."
    exit 1
fi
