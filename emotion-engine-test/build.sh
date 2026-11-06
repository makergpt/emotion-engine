#!/bin/bash
# Exit immediately if a command exits with a non-zero status
set -e

echo "[INFO] Starting lightning-fast parallel build..."

# Create build directory if it doesn't exist
mkdir -p build && cd build

# Configure CMake or compile directly with core-scaling
g++ -std=c++17 -O3 -I../include \
    ../src/main.cpp \
    ../src/TranslationLayer.cpp \
    ../src/AppInterceptor.cpp \
    ../src/Renderer.cpp \
    -lGLEW -lglfw -lGL -ldl -lpthread \
    -o ../emotion-engine

echo "[SUCCESS] Build complete: ./emotion-engine"
