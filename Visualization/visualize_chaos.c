/**
 * KAOS-256 Visualization Generator
 * Maximum point density + proper speed controls
 */

#include "kaos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_POINTS 2000  // Suficiente para 1000 warmup + texto

typedef struct {
    double x, y, z;
    int step;
    char phase[20];
    uint8_t keystream_byte;
} ChaosPoint;

void generate_random_key(uint8_t* key, size_t size) {
    for (size_t i = 0; i < size; i++) {
        key[i] = rand() % 256;
    }
}

void generate_corrected_html(const char* plaintext, const uint8_t* key, 
                            const uint8_t* nonce, const char* output_file) {
    
    KaosCipher cipher;
    kaos_init(&cipher);
    cipher.warmup = 1000;
    
    int text_len = strlen(plaintext);
    ChaosPoint points[MAX_POINTS];
    int point_count = 0;
    
    printf("GENERATING HIGH-DENSITY VISUALIZATION...\n");
    printf("Text: '%s' (%d bytes)\n", plaintext, text_len);
    printf("Warmup iterations: %d\n", cipher.warmup);
    printf("Maximum points: %d\n\n", MAX_POINTS);
    
    // Initialize chaotic system
    double x, y, z;
    kaos_key_to_state(key, nonce, &x, &y, &z);
    
    printf("Phase 1: Warmup (capturing ALL points)...\n");
    
    // WARMUP: Capturar TODOS los puntos
    for (int i = 0; i < cipher.warmup && point_count < MAX_POINTS; i++) {
        lorenz_step(&cipher, &x, &y, &z);
        
        // CAPTURAR 100% de los puntos
        points[point_count].x = x;
        points[point_count].y = y;
        points[point_count].z = z;
        points[point_count].step = i;
        strcpy(points[point_count].phase, "warmup");
        points[point_count].keystream_byte = 0;
        point_count++;
        
        // Mostrar progreso
        if (i % 200 == 0 && i > 0) {
            printf("   Progress: %d/%d points\n", point_count, cipher.warmup);
        }
    }
    
    printf("Warmup completed: %d points\n", point_count);
    
    // ENCRYPTION: Capturar TODOS los puntos
    printf("Phase 2: Encryption (capturing ALL points)...\n");
    
    for (int i = 0; i < text_len && point_count < MAX_POINTS; i++) {
        lorenz_step(&cipher, &x, &y, &z);
        uint8_t k_byte = kaos_keystream_byte(x, y, z, i);
        
        // CAPTURAR 100% de los puntos
        points[point_count].x = x;
        points[point_count].y = y;
        points[point_count].z = z;
        points[point_count].step = cipher.warmup + i;
        strcpy(points[point_count].phase, "encryption");
        points[point_count].keystream_byte = k_byte;
        point_count++;
    }
    
    printf("Encryption completed: %d total points\n", point_count);
    
    // Generate HTML with embedded data
    printf("Creating HTML visualization...\n");
    FILE* f = fopen(output_file, "w");
    if (!f) {
        printf("Error creating output file\n");
        return;
    }
    
    // HTML con interfaz idéntica pero datos corregidos
    fprintf(f, "<!DOCTYPE html>\n");
    fprintf(f, "<html lang=\"en\">\n");
    fprintf(f, "<head>\n");
    fprintf(f, "    <meta charset=\"UTF-8\">\n");
    fprintf(f, "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
    fprintf(f, "    <title>KAOS Cipher: Lorenz Attractor</title>\n");
    fprintf(f, "    <script src=\"https://cdnjs.cloudflare.com/ajax/libs/three.js/r128/three.min.js\"></script>\n");
    fprintf(f, "    <script src=\"https://cdn.jsdelivr.net/npm/three@0.128.0/examples/js/controls/OrbitControls.min.js\"></script>\n");
    fprintf(f, "    <style>\n");
    fprintf(f, "        * {\n");
    fprintf(f, "            margin: 0;\n");
    fprintf(f, "            padding: 0;\n");
    fprintf(f, "            box-sizing: border-box;\n");
    fprintf(f, "        }\n");
    fprintf(f, "        \n");
    fprintf(f, "        body {\n");
    fprintf(f, "            font-family: 'Courier New', monospace;\n");
    fprintf(f, "            background: #0f0f23;\n");
    fprintf(f, "            color: #00ff00;\n");
    fprintf(f, "            overflow: hidden;\n");
    fprintf(f, "        }\n");
    fprintf(f, "        \n");
    fprintf(f, "        #container {\n");
    fprintf(f, "            position: relative;\n");
    fprintf(f, "            width: 100vw;\n");
    fprintf(f, "            height: 100vh;\n");
    fprintf(f, "        }\n");
    fprintf(f, "        \n");
    fprintf(f, "        #canvas-container {\n");
    fprintf(f, "            width: 100%%;\n");
    fprintf(f, "            height: 100%%;\n");
    fprintf(f, "        }\n");
    fprintf(f, "        \n");
    fprintf(f, "        #controls {\n");
    fprintf(f, "            position: absolute;\n");
    fprintf(f, "            top: 20px;\n");
    fprintf(f, "            left: 20px;\n");
    fprintf(f, "            background: rgba(0, 0, 0, 0.8);\n");
    fprintf(f, "            padding: 20px;\n");
    fprintf(f, "            border-radius: 10px;\n");
    fprintf(f, "            border: 1px solid #00ff00;\n");
    fprintf(f, "            z-index: 100;\n");
    fprintf(f, "        }\n");
    fprintf(f, "        \n");
    fprintf(f, "        .control-group {\n");
    fprintf(f, "            margin-bottom: 15px;\n");
    fprintf(f, "        }\n");
    fprintf(f, "        \n");
    fprintf(f, "        label {\n");
    fprintf(f, "            display: block;\n");
    fprintf(f, "            margin-bottom: 5px;\n");
    fprintf(f, "            font-weight: bold;\n");
    fprintf(f, "        }\n");
    fprintf(f, "        \n");
    fprintf(f, "        input[type=\"range\"] {\n");
    fprintf(f, "            width: 200px;\n");
    fprintf(f, "        }\n");
    fprintf(f, "        \n");
    fprintf(f, "        button {\n");
    fprintf(f, "            background: #00ff00;\n");
    fprintf(f, "            color: #000;\n");
    fprintf(f, "            border: none;\n");
    fprintf(f, "            padding: 8px 16px;\n");
    fprintf(f, "            margin: 5px;\n");
    fprintf(f, "            border-radius: 5px;\n");
    fprintf(f, "            cursor: pointer;\n");
    fprintf(f, "            font-family: 'Courier New', monospace;\n");
    fprintf(f, "            font-weight: bold;\n");
    fprintf(f, "        }\n");
    fprintf(f, "        \n");
    fprintf(f, "        button:hover {\n");
    fprintf(f, "            background: #00cc00;\n");
    fprintf(f, "        }\n");
    fprintf(f, "        \n");
    fprintf(f, "        #info-panel {\n");
    fprintf(f, "            position: absolute;\n");
    fprintf(f, "            top: 20px;\n");
    fprintf(f, "            right: 20px;\n");
    fprintf(f, "            background: rgba(0, 0, 0, 0.8);\n");
    fprintf(f, "            padding: 20px;\n");
    fprintf(f, "            border-radius: 10px;\n");
    fprintf(f, "            border: 1px solid #00ff00;\n");
    fprintf(f, "            max-width: 300px;\n");
    fprintf(f, "        }\n");
    fprintf(f, "        \n");
    fprintf(f, "        .phase-warmup { color: #ff4444; }\n");
    fprintf(f, "        .phase-encryption { color: #44ff44; }\n");
    fprintf(f, "        \n");
    fprintf(f, "        .keystream-byte {\n");
    fprintf(f, "            display: inline-block;\n");
    fprintf(f, "            width: 30px;\n");
    fprintf(f, "            text-align: center;\n");
    fprintf(f, "            background: #003300;\n");
    fprintf(f, "            margin: 2px;\n");
    fprintf(f, "            padding: 2px;\n");
    fprintf(f, "            border-radius: 3px;\n");
    fprintf(f, "        }\n");
    fprintf(f, "    </style>\n");
    fprintf(f, "</head>\n");
    fprintf(f, "<body>\n");
    fprintf(f, "    <div id=\"container\">\n");
    fprintf(f, "        <div id=\"canvas-container\"></div>\n");
    fprintf(f, "        \n");
    fprintf(f, "        <div id=\"controls\">\n");
    fprintf(f, "            <h2>🎮 KAOS Cipher Controls</h2>\n");
    fprintf(f, "            \n");
    fprintf(f, "            <div class=\"control-group\">\n");
    fprintf(f, "                <label for=\"speed\">Animation Speed: <span id=\"speed-value\">1x</span></label>\n");
    fprintf(f, "                <input type=\"range\" id=\"speed\" min=\"0.1\" max=\"5\" value=\"1\" step=\"0.1\">\n");
    fprintf(f, "            </div>\n");
    fprintf(f, "            \n");
    fprintf(f, "            <div class=\"control-group\">\n");
    fprintf(f, "                <label for=\"trail\">Trail Length: <span id=\"trail-value\">500</span></label>\n");
    fprintf(f, "                <input type=\"range\" id=\"trail\" min=\"100\" max=\"1500\" value=\"500\" step=\"50\">\n");
    fprintf(f, "            </div>\n");
    fprintf(f, "            \n");
    fprintf(f, "            <div class=\"control-group\">\n");
    fprintf(f, "                <button id=\"play-pause\">⏸️ Pause</button>\n");
    fprintf(f, "                <button id=\"restart\">🔄 Restart</button>\n");
    fprintf(f, "                <button id=\"loop\">🔁 Loop: OFF</button>\n");
    fprintf(f, "            </div>\n");
    fprintf(f, "            \n");
    fprintf(f, "            <div class=\"control-group\">\n");
    fprintf(f, "                <label>Camera Presets:</label>\n");
    fprintf(f, "                <button data-preset=\"top\">Top View</button>\n");
    fprintf(f, "                <button data-preset=\"side\">Side View</button>\n");
    fprintf(f, "                <button data-preset=\"orbit\">Orbit Mode</button>\n");
    fprintf(f, "            </div>\n");
    fprintf(f, "        </div>\n");
    fprintf(f, "        \n");
    fprintf(f, "        <div id=\"info-panel\">\n");
    fprintf(f, "            <h3>📊 Chaos Information</h3>\n");
    fprintf(f, "            <div id=\"step-info\">Step: <span>0</span></div>\n");
    fprintf(f, "            <div id=\"phase-info\">Phase: <span class=\"phase-warmup\">WARMUP</span></div>\n");
    fprintf(f, "            <div id=\"position-info\">\n");
    fprintf(f, "                Position: <br>\n");
    fprintf(f, "                X: <span id=\"pos-x\">0.000</span><br>\n");
    fprintf(f, "                Y: <span id=\"pos-y\">0.000</span><br>\n");
    fprintf(f, "                Z: <span id=\"pos-z\">0.000</span>\n");
    fprintf(f, "            </div>\n");
    fprintf(f, "            <div id=\"keystream-info\">\n");
    fprintf(f, "                Keystream Byte: <span class=\"keystream-byte\">0</span>\n");
    fprintf(f, "            </div>\n");
    fprintf(f, "            <div id=\"stats\">\n");
    fprintf(f, "                Points: <span id=\"points-current\">0</span>/<span id=\"points-total\">%d</span><br>\n", point_count);
    fprintf(f, "                FPS: <span>0</span>\n");
    fprintf(f, "            </div>\n");
    fprintf(f, "        </div>\n");
    fprintf(f, "    </div>\n");
    fprintf(f, "\n");
    fprintf(f, "    <script>\n");
    fprintf(f, "        // ===== DATOS REALES CORREGIDOS =====\n");
    fprintf(f, "        const chaosData = {\n");
    fprintf(f, "            metadata: {\n");
    fprintf(f, "                totalPoints: %d,\n", point_count);
    fprintf(f, "                plaintext: \"%s\",\n", plaintext);
    fprintf(f, "                textLength: %d,\n", text_len);
    fprintf(f, "                warmupIterations: %d\n", cipher.warmup);
    fprintf(f, "            },\n");
    fprintf(f, "            trajectory: [\n");
    
    // Embed all trajectory points
    for (int i = 0; i < point_count; i++) {
        fprintf(f, "                {x: %.6f, y: %.6f, z: %.6f, step: %d, phase: \"%s\", keystream: %d}%s\n",
                points[i].x, points[i].y, points[i].z, points[i].step, 
                points[i].phase, points[i].keystream_byte,
                (i < point_count - 1) ? "," : "");
    }
    
    fprintf(f, "            ]\n");
    fprintf(f, "        };\n");
    fprintf(f, "        \n");
    fprintf(f, "        console.log(\"DATA LOADED:\", chaosData.trajectory.length, \"points\");\n");
    fprintf(f, "        console.log(\"Encrypted text:\", chaosData.metadata.plaintext);\n");
    fprintf(f, "        \n");
    fprintf(f, "        // ===== THREE.JS VISUALIZATION CORREGIDA =====\n");
    fprintf(f, "        class CorrectedChaosVisualizer {\n");
    fprintf(f, "            constructor() {\n");
    fprintf(f, "                this.scene = new THREE.Scene();\n");
    fprintf(f, "                this.camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);\n");
    fprintf(f, "                this.renderer = new THREE.WebGLRenderer({ antialias: true, alpha: true });\n");
    fprintf(f, "                \n");
    fprintf(f, "                this.trajectory = chaosData.trajectory;\n");
    fprintf(f, "                this.currentPoint = 0;\n");
    fprintf(f, "                this.animationId = null;\n");
    fprintf(f, "                this.isPlaying = true;\n");
    fprintf(f, "                this.speed = 1.0;  // Velocidad como float\n");
    fprintf(f, "                this.accumulator = 0.0;  // Acumulador para velocidad fraccional\n");
    fprintf(f, "                this.trailLength = 500;\n");
    fprintf(f, "                this.loop = false;\n");
    fprintf(f, "                \n");
    fprintf(f, "                this.init();\n");
    fprintf(f, "            }\n");
    fprintf(f, "            \n");
    fprintf(f, "            init() {\n");
    fprintf(f, "                // Setup renderer\n");
    fprintf(f, "                this.renderer.setSize(window.innerWidth, window.innerHeight);\n");
    fprintf(f, "                this.renderer.setClearColor(0x0f0f23, 1);\n");
    fprintf(f, "                document.getElementById('canvas-container').appendChild(this.renderer.domElement);\n");
    fprintf(f, "                \n");
    fprintf(f, "                // Setup camera\n");
    fprintf(f, "                this.camera.position.set(0, 0, 50);\n");
    fprintf(f, "                \n");
    fprintf(f, "                // Controls\n");
    fprintf(f, "                this.controls = new THREE.OrbitControls(this.camera, this.renderer.domElement);\n");
    fprintf(f, "                this.controls.enableDamping = true;\n");
    fprintf(f, "                this.controls.dampingFactor = 0.05;\n");
    fprintf(f, "                \n");
    fprintf(f, "                // Lighting\n");
    fprintf(f, "                const ambientLight = new THREE.AmbientLight(0x404040);\n");
    fprintf(f, "                this.scene.add(ambientLight);\n");
    fprintf(f, "                \n");
    fprintf(f, "                const directionalLight = new THREE.DirectionalLight(0xffffff, 1);\n");
    fprintf(f, "                directionalLight.position.set(50, 50, 50);\n");
    fprintf(f, "                this.scene.add(directionalLight);\n");
    fprintf(f, "                \n");
    fprintf(f, "                // Attractor trail\n");
    fprintf(f, "                this.trailGeometry = new THREE.BufferGeometry();\n");
    fprintf(f, "                this.trailMaterial = new THREE.LineBasicMaterial({ \n");
    fprintf(f, "                    color: 0x00ff00,\n");
    fprintf(f, "                    transparent: true,\n");
    fprintf(f, "                    opacity: 0.8\n");
    fprintf(f, "                });\n");
    fprintf(f, "                this.trail = new THREE.Line(this.trailGeometry, this.trailMaterial);\n");
    fprintf(f, "                this.scene.add(this.trail);\n");
    fprintf(f, "                \n");
    fprintf(f, "                // Current position marker\n");
    fprintf(f, "                const markerGeometry = new THREE.SphereGeometry(0.5, 16, 16);\n");
    fprintf(f, "                const markerMaterial = new THREE.MeshBasicMaterial({ color: 0xff4444 });\n");
    fprintf(f, "                this.marker = new THREE.Mesh(markerGeometry, markerMaterial);\n");
    fprintf(f, "                this.scene.add(this.marker);\n");
    fprintf(f, "                \n");
    fprintf(f, "                // Axes helper\n");
    fprintf(f, "                const axesHelper = new THREE.AxesHelper(20);\n");
    fprintf(f, "                this.scene.add(axesHelper);\n");
    fprintf(f, "                \n");
    fprintf(f, "                // Event listeners\n");
    fprintf(f, "                this.setupControls();\n");
    fprintf(f, "                this.animate();\n");
    fprintf(f, "                \n");
    fprintf(f, "                // Mostrar puntos totales\n");
    fprintf(f, "                document.getElementById('points-total').textContent = this.trajectory.length;\n");
    fprintf(f, "            }\n");
    fprintf(f, "            \n");
    fprintf(f, "            setupControls() {\n");
    fprintf(f, "                // Control de velocidad MEJORADO\n");
    fprintf(f, "                document.getElementById('speed').addEventListener('input', (e) => {\n");
    fprintf(f, "                    this.speed = parseFloat(e.target.value);\n");
    fprintf(f, "                    document.getElementById('speed-value').textContent = this.speed.toFixed(1) + 'x';\n");
    fprintf(f, "                });\n");
    fprintf(f, "                \n");
    fprintf(f, "                document.getElementById('trail').addEventListener('input', (e) => {\n");
    fprintf(f, "                    this.trailLength = parseInt(e.target.value);\n");
    fprintf(f, "                    document.getElementById('trail-value').textContent = this.trailLength;\n");
    fprintf(f, "                    this.updateTrail();\n");
    fprintf(f, "                });\n");
    fprintf(f, "                \n");
    fprintf(f, "                document.getElementById('play-pause').addEventListener('click', () => {\n");
    fprintf(f, "                    this.isPlaying = !this.isPlaying;\n");
    fprintf(f, "                    document.getElementById('play-pause').textContent = \n");
    fprintf(f, "                        this.isPlaying ? '⏸️ Pause' : '▶️ Play';\n");
    fprintf(f, "                });\n");
    fprintf(f, "                \n");
    fprintf(f, "                document.getElementById('restart').addEventListener('click', () => {\n");
    fprintf(f, "                    this.currentPoint = 0;\n");
    fprintf(f, "                    this.accumulator = 0.0;\n");
    fprintf(f, "                    this.updateTrail();\n");
    fprintf(f, "                    this.isPlaying = true;\n");
    fprintf(f, "                    document.getElementById('play-pause').textContent = '⏸️ Pause';\n");
    fprintf(f, "                });\n");
    fprintf(f, "                \n");
    fprintf(f, "                document.getElementById('loop').addEventListener('click', () => {\n");
    fprintf(f, "                    this.loop = !this.loop;\n");
    fprintf(f, "                    document.getElementById('loop').textContent = \n");
    fprintf(f, "                        this.loop ? '🔁 Loop: ON' : '🔁 Loop: OFF';\n");
    fprintf(f, "                });\n");
    fprintf(f, "                \n");
    fprintf(f, "                // Camera presets\n");
    fprintf(f, "                document.querySelectorAll('[data-preset]').forEach(btn => {\n");
    fprintf(f, "                    btn.addEventListener('click', (e) => {\n");
    fprintf(f, "                        const preset = e.target.dataset.preset;\n");
    fprintf(f, "                        this.setCameraPreset(preset);\n");
    fprintf(f, "                    });\n");
    fprintf(f, "                });\n");
    fprintf(f, "            }\n");
    fprintf(f, "            \n");
    fprintf(f, "            setCameraPreset(preset) {\n");
    fprintf(f, "                switch(preset) {\n");
    fprintf(f, "                    case 'top':\n");
    fprintf(f, "                        this.camera.position.set(0, 50, 0);\n");
    fprintf(f, "                        this.controls.target.set(0, 0, 0);\n");
    fprintf(f, "                        break;\n");
    fprintf(f, "                    case 'side':\n");
    fprintf(f, "                        this.camera.position.set(50, 0, 0);\n");
    fprintf(f, "                        this.controls.target.set(0, 0, 0);\n");
    fprintf(f, "                        break;\n");
    fprintf(f, "                    case 'orbit':\n");
    fprintf(f, "                        this.camera.position.set(30, 30, 30);\n");
    fprintf(f, "                        this.controls.target.set(0, 0, 0);\n");
    fprintf(f, "                        break;\n");
    fprintf(f, "                }\n");
    fprintf(f, "                this.controls.update();\n");
    fprintf(f, "            }\n");
    fprintf(f, "            \n");
    fprintf(f, "            updateTrail() {\n");
    fprintf(f, "                if (!this.trajectory || this.trajectory.length === 0) return;\n");
    fprintf(f, "                \n");
    fprintf(f, "                const start = Math.max(0, this.currentPoint - this.trailLength);\n");
    fprintf(f, "                const end = this.currentPoint + 1;  // Incluir punto actual\n");
    fprintf(f, "                const segment = this.trajectory.slice(start, end);\n");
    fprintf(f, "                \n");
    fprintf(f, "                const positions = new Float32Array(segment.length * 3);\n");
    fprintf(f, "                segment.forEach((point, i) => {\n");
    fprintf(f, "                    positions[i * 3] = point.x;\n");
    fprintf(f, "                    positions[i * 3 + 1] = point.y;\n");
    fprintf(f, "                    positions[i * 3 + 2] = point.z;\n");
    fprintf(f, "                });\n");
    fprintf(f, "                \n");
    fprintf(f, "                this.trailGeometry.setAttribute('position', new THREE.BufferAttribute(positions, 3));\n");
    fprintf(f, "                this.trailGeometry.attributes.position.needsUpdate = true;\n");
    fprintf(f, "                \n");
    fprintf(f, "                // Update marker position\n");
    fprintf(f, "                const current = this.trajectory[Math.floor(this.currentPoint)];\n");
    fprintf(f, "                if (current) {\n");
    fprintf(f, "                    this.marker.position.set(current.x, current.y, current.z);\n");
    fprintf(f, "                    \n");
    fprintf(f, "                    // Update info panel\n");
    fprintf(f, "                    document.getElementById('step-info').children[0].textContent = current.step;\n");
    fprintf(f, "                    document.getElementById('phase-info').children[0].textContent = current.phase.toUpperCase();\n");
    fprintf(f, "                    document.getElementById('phase-info').children[0].className = `phase-${current.phase}`;\n");
    fprintf(f, "                    document.getElementById('pos-x').textContent = current.x.toFixed(3);\n");
    fprintf(f, "                    document.getElementById('pos-y').textContent = current.y.toFixed(3);\n");
    fprintf(f, "                    document.getElementById('pos-z').textContent = current.z.toFixed(3);\n");
    fprintf(f, "                    document.getElementById('keystream-info').children[0].textContent = current.keystream;\n");
    fprintf(f, "                    document.getElementById('points-current').textContent = Math.floor(this.currentPoint + 1);\n");
    fprintf(f, "                }\n");
    fprintf(f, "            }\n");
    fprintf(f, "            \n");
    fprintf(f, "            animate() {\n");
    fprintf(f, "                this.animationId = requestAnimationFrame(() => this.animate());\n");
    fprintf(f, "                \n");
    fprintf(f, "                if (this.isPlaying && this.trajectory && this.trajectory.length > 0) {\n");
    fprintf(f, "                    // Velocidad con acumulador para valores fraccionales\n");
    fprintf(f, "                    this.accumulator += this.speed;\n");
    fprintf(f, "                    \n");
    fprintf(f, "                    if (this.accumulator >= 1.0) {\n");
    fprintf(f, "                        const steps = Math.floor(this.accumulator);\n");
    fprintf(f, "                        this.accumulator -= steps;\n");
    fprintf(f, "                        this.currentPoint += steps;\n");
    fprintf(f, "                    }\n");
    fprintf(f, "                    \n");
    fprintf(f, "                    if (this.currentPoint >= this.trajectory.length) {\n");
    fprintf(f, "                        if (this.loop) {\n");
    fprintf(f, "                            this.currentPoint = 0;\n");
    fprintf(f, "                            this.accumulator = 0.0;\n");
    fprintf(f, "                        } else {\n");
    fprintf(f, "                            this.currentPoint = this.trajectory.length - 1;\n");
    fprintf(f, "                            this.isPlaying = false;\n");
    fprintf(f, "                            document.getElementById('play-pause').textContent = '▶️ Play';\n");
    fprintf(f, "                        }\n");
    fprintf(f, "                    }\n");
    fprintf(f, "                    \n");
    fprintf(f, "                    this.updateTrail();\n");
    fprintf(f, "                }\n");
    fprintf(f, "                \n");
    fprintf(f, "                this.controls.update();\n");
    fprintf(f, "                this.renderer.render(this.scene, this.camera);\n");
    fprintf(f, "                \n");
    fprintf(f, "                // Update FPS counter\n");
    fprintf(f, "                this.updateFPS();\n");
    fprintf(f, "            }\n");
    fprintf(f, "            \n");
    fprintf(f, "            updateFPS() {\n");
    fprintf(f, "                if (!this.lastTime) {\n");
    fprintf(f, "                    this.lastTime = performance.now();\n");
    fprintf(f, "                    this.frameCount = 0;\n");
    fprintf(f, "                    return;\n");
    fprintf(f, "                }\n");
    fprintf(f, "                \n");
    fprintf(f, "                this.frameCount++;\n");
    fprintf(f, "                const currentTime = performance.now();\n");
    fprintf(f, "                if (currentTime >= this.lastTime + 1000) {\n");
    fprintf(f, "                    document.getElementById('stats').children[1].textContent = \n");
    fprintf(f, "                        Math.round((this.frameCount * 1000) / (currentTime - this.lastTime));\n");
    fprintf(f, "                    this.frameCount = 0;\n");
    fprintf(f, "                    this.lastTime = currentTime;\n");
    fprintf(f, "                }\n");
    fprintf(f, "            }\n");
    fprintf(f, "        }\n");
    fprintf(f, "        \n");
    fprintf(f, "        // Initialize when page loads\n");
    fprintf(f, "        window.addEventListener('load', () => {\n");
    fprintf(f, "            new CorrectedChaosVisualizer();\n");
    fprintf(f, "        });\n");
    fprintf(f, "        \n");
    fprintf(f, "        // Handle window resize\n");
    fprintf(f, "        window.addEventListener('resize', () => {\n");
    fprintf(f, "            if (window.visualizer) {\n");
    fprintf(f, "                window.visualizer.camera.aspect = window.innerWidth / window.innerHeight;\n");
    fprintf(f, "                window.visualizer.camera.updateProjectionMatrix();\n");
    fprintf(f, "                window.visualizer.renderer.setSize(window.innerWidth, window.innerHeight);\n");
    fprintf(f, "            }\n");
    fprintf(f, "        });\n");
    fprintf(f, "    </script>\n");
    fprintf(f, "</body>\n");
    fprintf(f, "</html>\n");
    
    fclose(f);
    
    printf("HTML CREATED: %s\n", output_file);
    printf("High-density points: %d\n", point_count);
    printf("Should show proper Lorenz butterfly pattern\n");
}

int main() {
    printf("===============================================\n");
    printf("             KAOS-256 VISUALIZATION             \n");
    printf("===============================================\n\n");
    
    srand(time(NULL));
    
    const char* demo_text = "KAOS CIPHER SECURE ENCRYPTION";
    uint8_t key[KAOS_KEY_SIZE];
    uint8_t nonce[KAOS_NONCE_SIZE];
    
    generate_random_key(key, KAOS_KEY_SIZE);
    generate_random_key(nonce, KAOS_NONCE_SIZE);
    
    printf("Demo Configuration:\n");
    printf("  Text: '%s'\n", demo_text);
    printf("  Key: 256-bit random\n");
    printf("  Nonce: 96-bit random\n\n");
    
    generate_corrected_html(demo_text, key, nonce, "animation.html");
    
    printf("\n   VISUALIZATION READY!\n");
    printf("   File: animation.html\n");
    printf("   Features:\n");
    printf("   * 100%% point density (1028+ points)\n");
    printf("   * Smooth speed control (0.1x to 5x)\n"); 
    printf("   * Proper fractional speed handling\n");
    printf("   * Real Lorenz butterfly pattern\n");
    
    return 0;
}