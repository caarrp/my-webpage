class EnhancedWaveShadows : public WaveShadowGenerator {
public:
    EnhancedWaveShadows(int w, int h) : WaveShadowGenerator(w, h) {}
    
    std::vector<float> computeEnhancedShadows(const Vector3& lightDir, float time) {
        Vector3 L = lightDir.normalize();
        Vector3 viewDir(0, 0, 1); // Looking straight down
        std::vector<float> result(width * height);
        
        for (int i = 0; i < width * height; i++) {
            Vector3 N = normals[i];
            
            // 1. Diffuse component (Lambertian)
            float diffuse = std::max(0.0f, N.x * L.x + N.y * L.y + N.z * L.z);
            
            // 2. Specular component (sun glint)
            Vector3 H = (L + viewDir).normalize();
            float specular = std::pow(std::max(0.0f, N.x * H.x + N.y * H.y + N.z * H.z), 32.0f);
            
            // 3. Depth-based darkening
            float depth = 1.0f - (heightField[i] - getMinHeight()) / (getMaxHeight() - getMinHeight());
            float depthEffect = 0.7f + 0.3f * depth;
            
            // 4. Simple ambient occlusion (neighbors block light)
            float occlusion = computeAmbientOcclusion(i);
            
            // Combine all effects
            result[i] = std::clamp(diffuse * depthEffect * occlusion + 0.2f * specular, 0.1f, 1.0f);
        }
        return result;
    }
    
private:
    float computeAmbientOcclusion(int index) {
        int x = index % width;
        int y = index / width;
        
        if (x < 2 || x >= width-2 || y < 2 || y >= height-2) 
            return 1.0f;
            
        float centerHeight = heightField[index];
        float occlusion = 0.0f;
        int samples = 0;
        
        // Sample 3x3 neighborhood
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dx == 0 && dy == 0) continue;
                
                int neighborIdx = (y + dy) * width + (x + dx);
                float neighborHeight = heightField[neighborIdx];
                
                // If neighbor is significantly higher, it occludes light
                if (neighborHeight > centerHeight + 0.1f) {
                    occlusion += 0.1f;
                }
                samples++;
            }
        }
        
        return 1.0f - (occlusion / samples);
    }
    
    float getMinHeight() {
        return *std::min_element(heightField.begin(), heightField.end());
    }
    
    float getMaxHeight() {
        return *std::max_element(heightField.begin(), heightField.end());
    }
};
