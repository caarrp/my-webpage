class WaveSimulation {
    constructor() {
        this.waveSolver = null;
        this.heightDataPtr = null;
        this.heightData = null;
    }

    async init() {
        // Load WebAssembly module
        const Module = await import('./build/waves.js');
        this.Module = Module;
        
        // Create C++ wave solver
        this.waveSolver = new Module.WaveSolver(512, 512);
        this.waveSolver.generateInitialWaves();
        
        return this;
    }

    update(dt) {
        if (this.waveSolver) {
            this.waveSolver.update(dt);
            
            // Get pointer to height data
            this.heightDataPtr = this.waveSolver.getHeightData();
            const dataSize = this.waveSolver.getDataSize();
            
            // Copy data to JavaScript array
            this.heightData = new Float32Array(
                this.Module.HEAPF32.buffer,
                this.heightDataPtr,
                dataSize
            );
        }
    }

    addDisturbance(x, y, strength) {
        if (this.waveSolver) {
            this.waveSolver.addDisturbance(x, y, strength);
        }
    }

    getHeightData() {
        return this.heightData;
    }
}
