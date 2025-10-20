#include <vector>
#include <cmath>
#include <algorithm>

struct Vector3 {
    float x, y, z;
    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    
    Vector3 normalize() const {
        float len = std::sqrt(x*x + y*y + z*z);
        return Vector3(x/len, y/len, z/len);
    }
};

class WaveShadowGenerator {
private:
    int width, height;
    std::vector<float> heightField;
    std::vector<Vector3> normals;
    
public:
    WaveShadowGenerator(int w, int h) : width(w), height(h) {
        heightField.resize(w * h);
        normals.resize(w * h);
    }
    
    // Generate wave height field using multiple sine waves
    void generateWaveHeightField(float time = 0.0f) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                float fx = (x - width/2.0f) / (width/8.0f);
                float fy = (y - height/2.0f) / (height/8.0f);
                
                // Superpose multiple waves
                float height = 0.0f;
                height += 0.5f * std::sin(fx * 2.0f + time) * std::sin(fy * 1.5f + time);
                height += 0.3f * std::sin(fx * 3.0f + time*1.3f) * std::sin(fy * 2.0f + time*0.7f);
                height += 0.2f * std::sin(fx * 0.7f + time*0.3f) * std::sin(fy * 1.2f + time*1.1f);
                
                heightField[y * width + x] = height;
            }
        }
        computeSurfaceNormals();
    }
    
    // Compute surface normals from height field
    void computeSurfaceNormals() {
        for (int y = 1; y < height - 1; y++) {
            for (int x = 1; x < width - 1; x++) {
                // Central differences for gradient
                float dx = heightField[y * width + (x + 1)] - heightField[y * width + (x - 1)];
                float dy = heightField[(y + 1) * width + x] - heightField[(y - 1) * width + x];
                
                // Normal vector: (-dx, -dy, 1.0)
                Vector3 normal(-dx, -dy, 2.0f); // Z component > 1 for steeper normals
                normals[y * width + x] = normal.normalize();
            }
        }
    }
    
    // Simple Lambertian shading
    std::vector<float> computeSimpleShadows(const Vector3& lightDir) {
        Vector3 L = lightDir.normalize();
        std::vector<float> shadows(width * height);
        
        for (int i = 0; i < width * height; i++) {
            float dot = normals[i].x * L.x + normals[i].y * L.y + normals[i].z * L.z;
            shadows[i] = std::clamp(dot, 0.1f, 1.0f);
        }
        return shadows;
    }
};
