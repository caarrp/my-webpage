class WebGLRenderer {
    constructor(canvas, waveSimulation) {
        this.gl = canvas.getContext('webgl');
        this.waveSim = waveSimulation;
        this.program = null;
        this.texture = null;
        
        this.initWebGL();
    }

    initWebGL() {
        // Vertex shader (same as before)
        const vsSource = `...`;
        
        // Fragment shader - now uses texture from C++
        const fsSource = `
            precision highp float;
            varying vec2 vUv;
            uniform sampler2D uHeightMap;
            uniform float uTime;
            uniform vec2 uMouse;
            uniform vec2 uResolution;
            
            vec3 computeNormal(sampler2D heightMap, vec2 uv) {
                vec2 texelSize = vec2(1.0) / vec2(512.0); // Match C++ resolution
                
                float hL = texture2D(heightMap, uv - vec2(texelSize.x, 0.0)).r;
                float hR = texture2D(heightMap, uv + vec2(texelSize.x, 0.0)).r;
                float hU = texture2D(heightMap, uv - vec2(0.0, texelSize.y)).r;
                float hD = texture2D(heightMap, uv + vec2(0.0, texelSize.y)).r;
                
                return normalize(vec3(hL - hR, hU - hD, 2.0));
            }
            
            void main() {
                vec2 uv = vUv;
                
                // Sample height from C++ simulation
                float height = texture2D(uHeightMap, uv).r;
                
                // Compute lighting using height map
                vec3 normal = computeNormal(uHeightMap, uv);
                vec3 lightDir = normalize(vec3(uMouse / uResolution - 0.5, 1.0));
                
                float diffuse = max(dot(normal, lightDir), 0.1);
                float specular = pow(max(dot(normal, lightDir), 0.0), 32.0);
                
                // Water coloring based on height
                vec3 color = mix(vec3(0.0, 0.1, 0.3), vec3(0.2, 0.5, 0.8), height + 0.5);
                color = color * diffuse + vec3(1.0) * specular * 0.3;
                
                gl_FragColor = vec4(color, 1.0);
            }
        `;
        
        this.program = this.createProgram(vsSource, fsSource);
        this.setupTexture();
    }

    setupTexture() {
        this.texture = this.gl.createTexture();
        this.gl.bindTexture(this.gl.TEXTURE_2D, this.texture);
        this.gl.texImage2D(this.gl.TEXTURE_2D, 0, this.gl.LUMINANCE, 
                          512, 512, 0, this.gl.LUMINANCE, this.gl.FLOAT, null);
        this.gl.texParameteri(this.gl.TEXTURE_2D, this.gl.TEXTURE_MIN_FILTER, this.gl.LINEAR);
        this.gl.texParameteri(this.gl.TEXTURE_2D, this.gl.TEXTURE_MAG_FILTER, this.gl.LINEAR);
        this.gl.texParameteri(this.gl.TEXTURE_2D, this.gl.TEXTURE_WRAP_S, this.gl.CLAMP_TO_EDGE);
        this.gl.texParameteri(this.gl.TEXTURE_2D, this.gl.TEXTURE_WRAP_T, this.gl.CLAMP_TO_EDGE);
    }

    updateTexture() {
        const heightData = this.waveSim.getHeightData();
        if (heightData) {
            this.gl.bindTexture(this.gl.TEXTURE_2D, this.texture);
            this.gl.texSubImage2D(this.gl.TEXTURE_2D, 0, 0, 0, 
                                512, 512, this.gl.LUMINANCE, this.gl.FLOAT, heightData);
        }
    }

    render(time, mousePos) {
        this.updateTexture();
        
        // Set uniforms and render...
        this.gl.uniform1f(this.timeUniform, time);
        this.gl.uniform2f(this.mouseUniform, mousePos[0], mousePos[1]);
        this.gl.uniform1i(this.heightMapUniform, 0);
        
        this.gl.drawArrays(this.gl.TRIANGLES, 0, 6);
    }
}
