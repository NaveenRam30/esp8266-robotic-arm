# Wi-Fi Controlled Robotic Arm (ESP8266)

## Overview

This project is a **4-DOF Wi-Fi-controlled robotic arm** built using a **NodeMCU (ESP8266)** and four SG90 micro servo motors.

The arm is controlled through a real-time web interface hosted directly on the ESP8266 using **WebSocket communication** for low-latency bidirectional control.

The system operates in **Access Point mode**, allowing direct connection from a smartphone or laptop without requiring an external router.

---

## Key Features

- Real-time WebSocket-based servo control  
- Onboard Wi-Fi Access Point (No router required)  
- Incremental motion logic for smooth mechanical movement  
- Dedicated battery-powered servo system  
- Bulk capacitor-based power stabilization  
- Fully embedded system (No cloud dependency)

---

## System Architecture

User Browser  
↓  
Web Interface (WebSocket)  
↓  
ESP8266 Web Server  
↓  
Motion Control Logic  
↓  
4 × SG90 Servos  

The ESP8266 hosts a lightweight HTTP server and WebSocket endpoint. Slider input from the browser sends angle commands, which are processed using constrained incremental motion logic before generating PWM signals for each servo.

---

## Hardware Used

- NodeMCU ESP8266 (ESP-12E Module)  
- 4 × SG90 Micro Servo Motors  
- 2 × 18500 Li-ion Batteries (Parallel Configuration)  
- TP4056 Li-ion Charging Module (with protection)  
- ~1200µF Capacitor Bank (power stabilization)  
- External USB supply for ESP8266 logic power  
- Breadboard and jumper wiring  

---

## Pin Configuration

| Function | ESP8266 Pin |
|----------|-------------|
| Claw (Gripper) | D4 |
| Top Arm | D3 |
| Bottom Arm | D7 |
| Base (Swing) | D8 |

---

## Motion Logic

### Claw (Gripper)
- Range: 0° – 45°  
- Step size: 1°  

### Base, Shoulder, Elbow
- Range: 0° – 180°  
- Step size: 3°  

Instead of directly writing slider values to servos, the system compares current and target positions and applies controlled incremental steps.

---

## Power Architecture

### Battery System

- 2 × 18500 Li-ion cells  
- Connected in parallel  
- Charged via TP4056 charging module  

Parallel configuration was selected to:
- Increase total capacity (mAh)  
- Improve current delivery capability  
- Reduce voltage sag under load  

---

### Power Flow

USB → TP4056 → 2×18500 (Parallel) → Servo Rail  
↓  
1200µF Capacitors  
↓  
4 × Servos  

The ESP8266 is powered separately through micro-USB to isolate logic power from motor noise.

---

## Voltage & Current Considerations

### Battery Parameters

- Nominal Voltage: 3.7V  
- Fully Charged: 4.2V  
- Configuration: Parallel  

### Servo Load

- Typical running current: 200–500mA per servo  
- Stall current: ~700mA per servo  
- Peak system draw: ~2–3A  

---

## Power Stability Improvements

During development, Wi-Fi instability occurred due to servo-induced voltage sag and ground bounce.

The issue was mitigated by:

- Adding ~1200µF bulk capacitance across servo rails  
- Implementing proper common-ground star topology  
- Separating motor and logic power  
- Using parallel Li-ion cells to reduce internal resistance  

This eliminated Wi-Fi dropouts and improved system stability under load.

---

## Software Stack

- ESP8266WiFi  
- ESPAsyncWebServer  
- ESPAsyncTCP  
- Servo Library  
- WebSocket-based communication  

**Board Selection:** NodeMCU 1.0 (ESP-12E Module)

---

## How to Run

1. Install required libraries:
   - ESPAsyncWebServer  
   - ESPAsyncTCP  
   - Servo  

2. Select board: NodeMCU 1.0 (ESP-12E Module)

3. Upload firmware.

4. Connect to Wi-Fi:
   - SSID: RobotArm  
   - Password: 12345678  

5. Open browser:
   http://192.168.4.1

---

## Engineering Challenges Solved

- Servo-induced Wi-Fi brownouts  
- Power rail noise suppression  
- Incremental servo motion control  
- Embedded WebSocket implementation  
- Resource optimization on ESP8266  

---

## Potential Improvements

- Add boost converter for stable 5–6V servo rail  
- Implement inverse kinematics  
- Add position memory presets  
- Introduce acceleration ramping  
- Migrate to ESP32 for expanded capability  

---

## Project Significance

This project demonstrates:

- Embedded systems firmware development  
- IoT-based device control  
- Power integrity design for motor systems  
- Real-time communication over WebSockets  
- Hardware-software integration  

---

## Author

**Naveen Ram**  
B.Tech Electronics and Communication Engineering  
Amrita Vishwa Vidyapeetham, Amritapuri  
Focus: Embedded Systems & IoT Development
