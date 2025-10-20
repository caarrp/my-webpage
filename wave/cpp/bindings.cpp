#include <emscripten/bind.h>
#include "wave_solver.hpp"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(WaveModule) {
    class_<WaveSolver>("WaveSolver")
        .constructor<int, int>()
        .function("update", &WaveSolver::update)
        .function("addDisturbance", &WaveSolver::addDisturbance)
        .function("generateInitialWaves", &WaveSolver::generateInitialWaves)
        .function("getHeightData", &WaveSolver::getHeightData, allow_raw_pointers())
        .function("getDataSize", &WaveSolver::getDataSize);
}
