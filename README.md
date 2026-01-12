# Escape Room Props Repository

Arduino/ESP sketches and projects for escape room prop development.

## Purpose

This repository contains all code for escape room props—interactive puzzle elements that players interact with during escape room experiences. Templates are provided as starting points for new props.

## Hardware

**Microcontrollers:**
- ESP32
- ESP8266
- Arduino boards

**Common Sensors & Components:**
- RFID readers (RC522, PN532)
- Keypads and buttons
- Reed switches and magnetic sensors
- IR sensors and break beams
- Load cells and pressure sensors
- Rotary encoders
- LED strips (WS2812B, etc.)
- Servos and solenoids for locks
- Audio modules (DFPlayer, etc.)

## Communication Protocols

**MQTT** - Primary messaging protocol for prop-to-controller communication
- Publish puzzle state changes
- Subscribe to game control commands (reset, solve, hints)
- Integration with Node-RED dashboards

**ESP-NOW** - Low-latency peer-to-peer communication
- Direct prop-to-prop communication without WiFi infrastructure
- Useful for chained puzzles or synchronized effects

**WiFi** - Network connectivity for MQTT and OTA updates

**Node-RED** - Central game controller and monitoring
- Dashboard for game masters
- Puzzle flow logic
- Timer and hint management

## Repository Structure

```
├── Templates/
│   └── ESP32/          # ESP32 PlatformIO template (you are here)
├── [prop sketches...]  # Individual prop projects
```

## Typical Prop Architecture

```
┌─────────────┐     MQTT      ┌───────────┐
│   ESP32     │◄─────────────►│  Node-RED │
│   Prop      │               │ Controller│
└─────────────┘               └───────────┘
      │                             │
      │ ESP-NOW                     │ Dashboard
      ▼                             ▼
┌─────────────┐               ┌───────────┐
│ Other Props │               │Game Master│
└─────────────┘               └───────────┘
```

## Build Commands (PlatformIO)

```bash
pio run                  # Build
pio run --target upload  # Upload to board
pio run --target clean   # Clean build
pio device monitor       # Serial monitor (115200 baud)
```
