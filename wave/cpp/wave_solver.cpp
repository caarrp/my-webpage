#include "wave_solver.hpp"

WaveSolver::WaveSolver(int w, int h) : width(w), height(h), damping(0.96f) {
    heightField.resize(width * height, 0.0f);
    velocityField.resize(width * height, 0.0f);
}

void WaveSolver::update(float dt) {
    std::vector<float> newHeight = heightField;
    std::vector<float> newVelocity = velocityField;
    
    // Simple wave equation: ∂²h/∂t² = c²∇²h - μ∂h/∂t
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int idx = y * width + x;
            
            // Laplacian (wave propagation)
            float laplacian = 
                heightField[idx - 1] + heightField[idx + 1] +
                heightField[idx - width] + heightField[idx + width] -
                4.0f * heightField[idx];
            
            // Wave equation with damping
            float acceleration = laplacian * 0.5f - velocityField[idx] * 0.02f;
            newVelocity[idx] += acceleration * dt;
            newHeight[idx] += newVelocity[idx] * dt;
            
            // Boundary damping
            if (x <= 2 || x >= width - 3 || y <= 2 || y >= height - 3) {
                newVelocity[idx] *= 0.8f;
            }
        }
    }
    
    heightField = newHeight;
    velocityField = newVelocity;
    
    // Global damping
    for (float& v : velocityField) v *= damping;
}

void WaveSolver::addDisturbance(int x, int y, float strength) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        int idx = y * width + x;
        velocityField[idx] += strength;
    }
}

void WaveSolver::generateInitialWaves() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float fx = (x - width/2.0f) / (width/8.0f);
            float fy = (y - height/2.0f) / (height/8.0f);
            
            heightField[y * width + x] = 
                0.3f * std::sin(fx) * std::sin(fy) +
                0.2f * std::sin(fx * 1.7f) * std::sin(fy * 1.3f);
        }
    }
}
