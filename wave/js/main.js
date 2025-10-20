class WaveApp {
    constructor() {
        this.canvas = document.getElementById('canvas');
        this.waveSim = new WaveSimulation();
        this.renderer = null;
        this.lastTime = 0;
    }

    async init() {
        await this.waveSim.init();
        this.renderer = new WebGLRenderer(this.canvas, this.waveSim);
        
        // Interactive disturbances
        this.canvas.addEventListener('click', (e) => {
            const rect = this.canvas.getBoundingClientRect();
            const x = (e.clientX - rect.left) / rect.width * 512;
            const y = (1.0 - (e.clientY - rect.top) / rect.height) * 512;
            this.waveSim.addDisturbance(Math.floor(x), Math.floor(y), 1.0);
        });
        
        this.animate();
    }

    animate(currentTime = 0) {
        const dt = (currentTime - this.lastTime) * 0.001;
        this.lastTime = currentTime;
        
        // Update C++ simulation
        this.waveSim.update(dt);
        
        // Render with WebGL
        this.renderer.render(currentTime * 0.001, this.mousePos);
        
        requestAnimationFrame((time) => this.animate(time));
    }
}

// Start application
new WaveApp().init();
