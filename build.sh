#!/bin/bash
emcc -O3 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' \
     -s EXPORTED_FUNCTIONS='["_malloc", "_free"]' \
     -Icpp/ cpp/wave_solver.cpp cpp/bindings.cpp \
     -o build/waves.js
