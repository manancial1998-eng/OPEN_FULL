<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Paulo I - Sistema de Monitoramento</title>
    <style>
        body {
            font-family: 'Arial', sans-serif;
            background-color: #1C1C1C;
            margin: 0;
            padding: 20px;
            color: #FFFAFA;
            display: flex;
            flex-direction: column;
            align-items: center;
            min-height: 100vh;
        }
        
        .container {
            background: #000;
            border: 2px solid;
            margin: 1rem;
            padding: 1rem;
            border-radius: 8px;
            text-align: center;
            width: 90%;
            max-width: 800px;
            box-shadow: 0 0 15px rgba(0, 255, 0, 0.2);
        }
        
        .sonar-container {
            border-color: #00FF00;
        }
        
        .motor-container {
            border-color: #FF4500;
        }
        
        .eletrica-container {
            border-color: #9370DB;
        }
        
        .controle-container {
            border-color: #1E90FF;
        }

        h1, h2 {
            color: #EAFAE3;
            text-align: center;
        }
        
        h2 {
            color: inherit;
            border-bottom: 1px solid;
            padding-bottom: 8px;
        }
        
        #sonarCanvas {
            background-color: #000;
            border: 1px solid #00FF00;
            border-radius: 8px;
            width: 100%;
            aspect-ratio: 2 / 1;
            image-rendering: -moz-crisp-edges;
            image-rendering: -webkit-optimize-contrast;
            image-rendering: pixelated;
            image-rendering: crisp-edges;
        }
        
        .sensor-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(150px, 1fr));
            gap: 8px;
            margin-top: 10px;
        }
        
        .sensor-data {
            background-color: rgba(0, 0, 0, 0.3);
            padding: 8px;
            border-radius: 5px;
            display: flex;
            justify-content: space-between;
            border-left: 4px solid;
            transition: all 0.3s ease;
        }
        
        .sensor-data:hover {
            background-color: rgba(0, 0, 0, 0.5);
            transform: scale(1.02);
        }
        
        .sensor-label {
            font-weight: bold;
        }
        
        .sensor-value {
            font-family: 'Courier New', monospace;
        }
        
        #connectBtn {
            background-color: #00FF00;
            color: #000;
            border: none;
            padding: 10px 20px;
            margin: 10px;
            font-weight: bold;
            cursor: pointer;
            border-radius: 5px;
            font-size: 16px;
            transition: all 0.3s ease;
            box-shadow: 0 0 10px rgba(0, 255, 0, 0.5);
        }
        
        #connectBtn:hover {
            background-color: #00CC00;
            transform: scale(1.05);
        }
        
        #simulateBtn {
            background-color: #1E90FF;
            color: #000;
            border: none;
            padding: 10px 20px;
            margin: 10px;
            font-weight: bold;
            cursor: pointer;
            border-radius: 5px;
            font-size: 16px;
            transition: all 0.3s ease;
            box-shadow: 0 0 10px rgba(30, 144, 255, 0.5);
        }
        
        #simulateBtn:hover {
            background-color: #006400;
            transform: scale(1.05);
        }
        
        #wifiBtn {
            background-color: #FF4500;
            color: #000;
            border: none;
            padding: 10px 20px;
            margin: 10px;
            font-weight: bold;
            cursor: pointer;
            border-radius: 5px;
            font-size: 16px;
            transition: all 0.3s ease;
            box-shadow: 0 0 10px rgba(255, 69, 0, 0.5);
        }
        
        #wifiBtn:hover {
            background-color: #CC3700;
            transform: scale(1.05);
        }
        
        .status-indicator {
            width: 12px;
            height: 12px;
            border-radius: 50%;
            display: inline-block;
            margin-right: 5px;
            box-shadow: 0 0 5px currentColor;
        }
        
        .status-good {
            background-color: #00FF00;
        }
        
        .status-warning {
            background-color: #FFFF00;
        }
        
        .status-danger {
            background-color: #FF0000;
        }
        
        .highlight {
            animation: pulse 2s infinite;
        }
        
        @keyframes pulse {
            0% { text-shadow: 0 0 5px currentColor; }
            50% { text-shadow: 0 0 15px currentColor; }
            100% { text-shadow: 0 0 5px currentColor; }
        }

        .gauge-container {
            display: flex;
            justify-content: space-around;
            margin-top: 20px;
            flex-wrap: wrap;
        }
        
        .gauge {
            width: 100px;
            height: 100px;
            position: relative;
            margin: 5px;
        }
        
        .gauge-circle {
            width: 100%;
            height: 100%;
            border-radius: 50%;
            position: relative;
            overflow: hidden;
            background: #111;
            border: 3px solid #333;
        }
        
        .gauge-title {
            position: absolute;
            top: 10px;
            width: 100%;
            text-align: center;
            font-weight: bold;
            color: #FFF;
        }
        
        .gauge-value {
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            font-size: 20px;
            font-weight: bold;
            color: #FFF;
            text-shadow: 0 0 5px rgba(0,0,0,0.7);
        }
        
        .gauge-unit {
            position: absolute;
            top: 65%;
            width: 100%;
            text-align: center;
            font-size: 14px;
            color: #AAA;
        }

        .unit {
            color: #ADD8E6;
            font-size: 0.9em;
            margin-left: 3px;
        }
        
        .connection-status {
            margin-top: 10px;
            padding: 5px 10px;
            border-radius: 5px;
            font-weight: bold;
        }
        
        .connected {
            background-color: #006400;
            color: #00FF00;
        }
        
        .disconnected {
            background-color: #8B0000;
            color: #FF0000;
        }
    </style>
</head>
<body>
    <h1>Paulo I - Sistema de Monitoramento</h1>
    
    <div>
        <button id="connectBtn">Conectar Serial</button>
        <button id="wifiBtn">Conectar WiFi</button>
        <button id="simulateBtn">Simular Dados</button>
    </div>
    
    <div id="connectionStatus" class="connection-status disconnected">Desconectado</div>
    
    <div class="container sonar-container">
        <h2>Sistema de Detecção</h2>
        <canvas id="sonarCanvas"></canvas>
        <div class="sensor-grid">
            <div class="sensor-data" style="border-left-color: #00FF00;">
                <span class="sensor-label">SONAR:</span>
                <span class="sensor-value" id="sonarDistance">--.-- m</span>
            </div>
        </div>
    </div>
    
    <div class="container motor-container">
        <h2>Propulsão</h2>
        <div class="sensor-grid">
            <div class="sensor-data" style="border-left-color: #FF4500;">
                <span class="sensor-label">Tensão Motor:</span>
                <span class="sensor-value" id="motorVolts">--.- V</span>
            </div>
            <div class="sensor-data" style="border-left-color: #FF4500;">
                <span class="sensor-label">Corrente Motor:</span>
                <span class="sensor-value" id="motorAmps">--.- A</span>
            </div>
        </div>
        
        <div class="gauge-container">
            <div class="gauge">
                <div class="gauge-circle" id="motorGauge">
                    <div class="gauge-title">Motor</div>
                    <div class="gauge-value" id="motorRpm">0</div>
                    <div class="gauge-unit">RPM</div>
                </div>
            </div>
            
            <div class="gauge">
                <div class="gauge-circle" id="heliceGauge">
                    <div class="gauge-title">Hélice</div>
                    <div class="gauge-value" id="heliceRpm">0</div>
                    <div class="gauge-unit">RPM</div>
                </div>
            </div>
        </div>
    </div>

    <div class="container eletrica-container">
        <h2>Sistema Elétrico</h2>
        <div class="sensor-grid">
            <div class="sensor-data" style="border-left-color: #9370DB;">
                <span class="sensor-label">Tensão Bateria:</span>
                <span class="sensor-value" id="batVolts">--.- V</span>
            </div>
            <div class="sensor-data" style="border-left-color: #9370DB;">
                <span class="sensor-label">Corrente Bateria:</span>
                <span class="sensor-value" id="batAmps">--.- A</span>
            </div>
        </div>
    </div>
    
    <div class="container controle-container">
        <h2>Controles</h2>
        <div class="sensor-grid">
            <div class="sensor-data" style="border-left-color: #1E90FF;">
                <span class="sensor-label">Acelerador:</span>
                <span class="sensor-value" id="aceleradorValue">--</span>
            </div>
        </div>
        
        <div class="switch-status">
            <div class="switch-option" id="switch1">Posição 1</div>
            <div class="switch-option" id="switch2">Posição 2</div>
            <div class="switch-option" id="switch3">Posição 3</div>
        </div>
    </div>

    <script>
        // sonar config
        const sonar = document.getElementById("sonarCanvas");
        const ctx = sonar.getContext("2d");
        function initCanvas() {
            sonar.width = sonar.offsetWidth;
            sonar.height = sonar.offsetWidth / 2;
            ctx.imageSmoothingEnabled = false;
            ctx.webkitImageSmoothingEnabled = false;
            ctx.mozImageSmoothingEnabled = false;
        }
        
        initCanvas();
        
        const config = {
            maxDistance: 12
        };

        let lastRenderTime = 0;
        const targetFPS = 30;
        let simulationInterval = null;
        let wifiInterval = null;
        let reader = null;
        let port = null;

        let sensorData = {
            sonar: { distance: null },
            motor: { rpm: null, volts: null, amps: null },
            helice: { rpm: null },
            eletrica: { volts: null, amps: null },
            controle: { acelerador: null, switch: null }
        };

        // Verificar suporte a WebSerial e WebSocket
        window.addEventListener('DOMContentLoaded', () => {
            if ('serial' in navigator) {
                document.getElementById('connectBtn').style.display = 'inline-block';
            } else {
                document.getElementById('connectBtn').style.display = 'none';
                document.getElementById('connectBtn').nextElementSibling.textContent = 'WebSerial não suportado';
            }
            
            document.getElementById('simulateBtn').style.display = 'inline-block';
            document.getElementById('wifiBtn').style.display = 'inline-block';
        });

        // ajuste layout
        window.addEventListener('resize', () => {
            initCanvas();
        });

        // Conectar via Serial (ESP32)
        document.getElementById('connectBtn').addEventListener('click', async () => {
            try {
                if (port) {
                    // Se já está conectado, desconectar
                    if (reader) {
                        await reader.cancel();
                        reader = null;
                    }
                    await port.close();
                    port = null;
                    updateConnectionStatus(false);
                    document.getElementById('connectBtn').textContent = 'Conectar Serial';
                    return;
                }
                
                port = await navigator.serial.requestPort();
                await port.open({ baudRate: 115200 });
                
                const textDecoder = new TextDecoderStream();
                const readableStreamClosed = port.readable.pipeTo(textDecoder.writable);
                reader = textDecoder.readable.getReader();
                
                document.getElementById('connectBtn').textContent = 'Desconectar';
                document.getElementById('connectBtn').style.backgroundColor = '#FF4500';
                updateConnectionStatus(true);
                
                // Parar simulações
                if (simulationInterval) {
                    clearInterval(simulationInterval);
                    simulationInterval = null;
                    document.getElementById('simulateBtn').textContent = 'Simular Dados';
                    document.getElementById('simulateBtn').style.backgroundColor = '#1E90FF';
                }
                
                // Parar WiFi se estiver ativo
                if (wifiInterval) {
                    clearInterval(wifiInterval);
                    wifiInterval = null;
                    document.getElementById('wifiBtn').textContent = 'Conectar WiFi';
                    document.getElementById('wifiBtn').style.backgroundColor = '#FF4500';
                }
                
                readSerialData();
            } catch (error) {
                console.error('Erro na conexão serial:', error);
                document.getElementById('connectBtn').textContent = 'Erro - Tentar novamente';
                document.getElementById('connectBtn').style.backgroundColor = '#FF0000';
            }
        });

        // Conectar via WiFi (ESP32 WebServer)
        document.getElementById('wifiBtn').addEventListener('click', async () => {
            if (wifiInterval) {
                clearInterval(wifiInterval);
                wifiInterval = null;
                document.getElementById('wifiBtn').textContent = 'Conectar WiFi';
                document.getElementById('wifiBtn').style.backgroundColor = '#FF4500';
                updateConnectionStatus(false);
                return;
            }
            
            document.getElementById('wifiBtn').textContent = 'Desconectar WiFi';
            document.getElementById('wifiBtn').style.backgroundColor = '#006400';
            updateConnectionStatus(true);
            
            // Buscar dados via HTTP a cada segundo
            wifiInterval = setInterval(fetchDataFromESP, 1000);
            fetchDataFromESP(); // Primeira chamada
        });

        // Simular dados
        document.getElementById('simulateBtn').addEventListener('click', () => {
            if (simulationInterval) {
                clearInterval(simulationInterval);
                simulationInterval = null;
                document.getElementById('simulateBtn').textContent = 'Simular Dados';
                document.getElementById('simulateBtn').style.backgroundColor = '#1E90FF';
            } else {
                document.getElementById('simulateBtn').textContent = 'Parar Simulação';
                document.getElementById('simulateBtn').style.backgroundColor = '#FF4500';
                
                generateSimulatedData();
                simulationInterval = setInterval(generateSimulatedData, 1000);
            }
        });

        async function readSerialData() {
            try {
                while (true) {
                    const { value, done } = await reader.read();
                    if (done) {
                        reader.releaseLock();
                        break;
                    }
                    
                    // Processar cada linha recebida
                    const lines = value.split('\n');
                    for (const line of lines) {
                        if (line.trim()) {
                            processSensorData(line.trim());
                        }
                    }
                }
            } catch (error) {
                console.error('Erro na leitura serial:', error);
                if (port) {
                    await port.close();
                    port = null;
                    reader = null;
                    updateConnectionStatus(false);
                    document.getElementById('connectBtn').textContent = 'Conectar Serial';
                    document.getElementById('connectBtn').style.backgroundColor = '#00FF00';
                }
            }
        }

        async function fetchDataFromESP() {
            try {
                const response = await fetch('http://192.168.4.1/dados');
                if (response.ok) {
                    const data = await response.text();
                    processSensorData(data);
                } else {
                    console.error('Erro ao buscar dados:', response.status);
                }
            } catch (error) {
                console.error('Erro na conexão WiFi:', error);
            }
        }

        function updateConnectionStatus(connected) {
            const statusElement = document.getElementById('connectionStatus');
            if (connected) {
                statusElement.textContent = 'Conectado';
                statusElement.className = 'connection-status connected';
            } else {
                statusElement.textContent = 'Desconectado';
                statusElement.className = 'connection-status disconnected';
            }
        }

        function generateSimulatedData() {
            const sonarDistance = (0.5 + Math.random() * 10).toFixed(2);
            const motorRpm = Math.floor(800 + Math.random() * 2200);
            const heliceRpm = Math.floor(motorRpm * (0.9 + Math.random() * 0.2));
            const motorVolts = (11 + Math.random() * 3).toFixed(1);
            const motorAmps = (5 + (motorRpm / 3000) * 15 + (Math.random() * 5 - 2.5)).toFixed(1);
            const batVolts = (11.5 + Math.random() * 2.5).toFixed(1);
            const batAmps = (5 + Math.random() * 15).toFixed(1);
            const acelerador = Math.floor(Math.random() * 511) - 255;
            const switchPos = Math.floor(Math.random() * 3) + 1;
            
            // Formatar conforme as strings do Arduino
            const simulatedData = 
                `SONAR:${sonarDistance}|MOTOR:${motorRpm}|HELICE:${heliceRpm}|` +
                `MOTOR_POW:${motorVolts},${motorAmps}|BATERIA:${batVolts},${batAmps}|` +
                `CONTROLE:${acelerador},${switchPos}`;
            
            processSensorData(simulatedData);
        }

        // processar dados dos sensores
        function processSensorData(data) {
            try {
                const parts = data.split('|');
                
                parts.forEach(part => {
                    if (!part) return;
                    
                    const [sensor, values] = part.split(':');
                    if (!values) return;
                    
                    switch(sensor) {
                        case 'SONAR':
                            sensorData.sonar.distance = parseFloat(values);
                            updateSonarDisplay();
                            break;
                            
                        case 'MOTOR':
                            sensorData.motor.rpm = parseInt(values);
                            updateMotorDisplay();
                            updateGauges();
                            break;
                            
                        case 'HELICE':
                            sensorData.helice.rpm = parseInt(values);
                            updateHeliceDisplay();
                            updateGauges();
                            break;
                            
                        case 'MOTOR_POW':
                            const [motV, motA] = values.split(',');
                            sensorData.motor = {
                                ...sensorData.motor,
                                volts: parseFloat(motV),
                                amps: parseFloat(motA)
                            };
                            updateMotorPowerDisplay();
                            break;
                            
                        case 'BATERIA':
                            const [batV, batA] = values.split(',');
                            sensorData.eletrica.volts = parseFloat(batV);
                            sensorData.eletrica.amps = parseFloat(batA);
                            updateBateriaDisplay();
                            break;
                            
                        case 'CONTROLE':
                            const [acelerador, switchPos] = values.split(',');
                            sensorData.controle = {
                                acelerador: parseInt(acelerador),
                                switch: parseInt(switchPos)
                            };
                            updateControleDisplay();
                            break;
                    }
                });
            } catch (error) {
                console.error('Erro ao processar dados:', error, 'Dados:', data);
            }
        }

        // atts
        function updateGauges() {
            const motorRpm = sensorData.motor.rpm || 0;
            document.getElementById("motorRpm").textContent = motorRpm;
            updateGaugeColor("motorGauge", motorRpm, 3000);
            
            const heliceRpm = sensorData.helice.rpm || 0;
            document.getElementById("heliceRpm").textContent = heliceRpm;
            updateGaugeColor("heliceGauge", heliceRpm, 3000);
        }

        function updateGaugeColor(gaugeId, value, max) {
            const gauge = document.getElementById(gaugeId);
            const percentage = Math.min(value / max, 1);
            
            // color layout
            let color;
            if (percentage < 0.5) {
                color = `hsl(50, 100%, ${50 - percentage * 20}%)`; // Amarelo escuro
            } else if (percentage < 0.8) {
                color = `hsl(40, 100%, ${50 - percentage * 15}%)`; // Laranja escuro
            } else {
                color = `hsl(30, 100%, ${50 - percentage * 10}%)`; // Vermelho escuro
            }
            
            gauge.style.background = `conic-gradient(${color} 0% ${percentage * 100}%, #333 ${percentage * 100}% 100%)`;
        }

        function updateSonarDisplay() {
            const element = document.getElementById('sonarDistance');
            if (sensorData.sonar.distance !== null) {
                element.textContent = sensorData.sonar.distance.toFixed(2) + ' m';
                
                if (sensorData.sonar.distance < 2) {
                    element.classList.add('highlight');
                } else {
                    element.classList.remove('highlight');
                }
            } else {
                element.textContent = '--.-- m';
                element.classList.remove('highlight');
            }
        }

        function updateMotorDisplay() {
            const element = document.getElementById('motorRpm');
            element.textContent = 
                sensorData.motor.rpm !== null ? sensorData.motor.rpm : '----';
            
            if (sensorData.motor.rpm !== null && sensorData.motor.rpm > 3000) {
                element.classList.add('highlight');
            } else {
                element.classList.remove('highlight');
            }
        }

        function updateHeliceDisplay() {
            document.getElementById('heliceRpm').textContent = 
                sensorData.helice.rpm !== null ? sensorData.helice.rpm : '----';
        }

        function updateMotorPowerDisplay() {
            document.getElementById('motorVolts').textContent = 
                sensorData.motor.volts !== null ? sensorData.motor.volts.toFixed(1) + ' V' : '--.- V';
            document.getElementById('motorAmps').textContent = 
                sensorData.motor.amps !== null ? sensorData.motor.amps.toFixed(1) + ' A' : '--.- A';
        }

        function updateBateriaDisplay() {
            const voltsElement = document.getElementById('batVolts');
            const ampsElement = document.getElementById('batAmps');
            
            voltsElement.textContent = 
                sensorData.eletrica.volts !== null ? sensorData.eletrica.volts.toFixed(1) + ' V' : '--.- V';
            ampsElement.textContent = 
                sensorData.eletrica.amps !== null ? sensorData.eletrica.amps.toFixed(1) + ' A' : '--.- A';
            
            if (sensorData.eletrica.volts !== null && sensorData.eletrica.volts < 11) {
                voltsElement.classList.add('highlight');
            } else {
                voltsElement.classList.remove('highlight');
            }
        }

        function updateControleDisplay() {
            const aceleradorElement = document.getElementById('aceleradorValue');
            if (sensorData.controle.acelerador !== null) {
                aceleradorElement.textContent = sensorData.controle.acelerador;
                
                if (Math.abs(sensorData.controle.acelerador) > 200) {
                    aceleradorElement.classList.add('highlight');
                } else {
                    aceleradorElement.classList.remove('highlight');
                }
            } else {
                aceleradorElement.textContent = '--';
            }
            
            // Atualizar estado do switch
            const switch1 = document.getElementById('switch1');
            const switch2 = document.getElementById('switch2');
            const switch3 = document.getElementById('switch3');
            
            switch1.classList.remove('switch-active');
            switch2.classList.remove('switch-active');
            switch3.classList.remove('switch-active');
            
            if (sensorData.controle.switch === 1) {
                switch1.classList.add('switch-active');
            } else if (sensorData.controle.switch === 2) {
                switch2.classList.add('switch-active');
            } else if (sensorData.controle.switch === 3) {
                switch3.classList.add('switch-active');
            }
        }

        // sonar animation
        function drawSonar(timestamp) {
            if (!lastRenderTime || timestamp - lastRenderTime >= 1000/targetFPS) {
                ctx.clearRect(0, 0, sonar.width, sonar.height);
                
                const centerX = sonar.width / 2;
                const centerY = sonar.height;
                const scale = sonar.height / config.maxDistance;

                drawGrid(centerX, centerY, scale);
                drawSonarLine(centerX, centerY, scale);
                
                lastRenderTime = timestamp;
            }
            requestAnimationFrame(drawSonar);
        }

        function drawGrid(cx, cy, scale) {
            ctx.strokeStyle = "#006400";
            ctx.lineWidth = 1;
            ctx.setLineDash([5, 3]);
            
            for (let dist = 1; dist <= config.maxDistance; dist++) {
                const radius = dist * scale;
                ctx.beginPath();
                ctx.arc(cx, cy, radius, Math.PI, 0, false);
                ctx.stroke();
                
                if (dist % 3 === 0) {
                    ctx.fillStyle = "#00FF00";
                    ctx.font = "12px Arial";
                    ctx.fillText(`${dist}m`, cx + radius + 5, cy - 5);
                }
            }
            
            ctx.setLineDash([]);
            ctx.beginPath();
            ctx.moveTo(cx, cy);
            ctx.lineTo(cx, cy - sonar.height);
            ctx.strokeStyle = "#00FF00";
            ctx.lineWidth = 1;
            ctx.stroke();
            
            for (let ang = -45; ang <= 45; ang += 15) {
                if (ang === 0) continue;
                
                const rad = (ang * Math.PI) / 180;
                ctx.beginPath();
                ctx.moveTo(cx, cy);
                ctx.lineTo(cx + sonar.height * Math.sin(rad), 
                          cy - sonar.height * Math.cos(rad));
                ctx.strokeStyle = "#006400";
                ctx.stroke();
                
                const textX = cx + (sonar.height - 20) * Math.sin(rad);
                const textY = cy - (sonar.height - 20) * Math.cos(rad);
                ctx.fillText(`${Math.abs(ang)}°`, textX, textY);
            }
        }

        function drawSonarLine(cx, cy, scale) {
            ctx.beginPath();
            ctx.moveTo(cx, cy);
            ctx.lineTo(cx, cy - sonar.height);
            ctx.strokeStyle = "rgba(0, 255, 0, 0.5)";
            ctx.lineWidth = 2;
            ctx.stroke();
            
            // objetos
            if (sensorData.sonar.distance !== null) {
                const objectDist = sensorData.sonar.distance * scale;
                const objectY = cy - objectDist;
                
                ctx.beginPath();
                ctx.moveTo(cx, cy);
                ctx.lineTo(cx, objectY);
                ctx.strokeStyle = "#FF0000";
                ctx.lineWidth = 1;
                ctx.stroke();
                
                ctx.beginPath();
                ctx.arc(cx, objectY, 5, 0, Math.PI * 2);
                ctx.fillStyle = "#FF0000";
                ctx.shadowBlur = 15;
                ctx.shadowColor = "#FF0000";
                ctx.fill();
                ctx.shadowBlur = 0;
                
                ctx.fillStyle = "#FFFFFF";
                ctx.font = "14px Arial";
                ctx.fillText(`${sensorData.sonar.distance.toFixed(1)}m`, cx + 10, objectY - 10);
            }
        }

        // start
        initCanvas();
        drawSonar();
    </script>
</body>
</html>