#ifndef WAVE_SOLVER_HPP
#define WAVE_SOLVER_HPP

#include <vector>
#include <cmath>

class WaveSolver {
private:
    int width, height;
    std::vector<float> heightField;
    std::vector<float> velocityField;
    float damping;
    
public:
    WaveSolver(int w, int h);
    
    // Physics simulation
    void update(float dt);
    void addDisturbance(int x, int y, float strength);
    
    // Data access for JavaScript
    const float* getHeightData() const { return heightField.data(); }
    int getDataSize() const { return width * height; }
    
    // Wave generation
    void generateInitialWaves();
};

#endif
