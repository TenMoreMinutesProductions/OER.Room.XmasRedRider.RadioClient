# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

ESP32-based escape room prop controller using PlatformIO with Arduino framework. Target board: `esp32dev`. This is a modular template where features are enabled/disabled via configuration flags.

## Build Commands

```bash
pio run                    # Build
pio run --target upload    # Upload to ESP32
pio run --target clean     # Clean build
pio device monitor         # Serial monitor (115200 baud)
```

## Architecture

### Configuration-Driven Design

All configuration lives in `src/config.h`. This is the primary entry point for customization:
- `DEVICE_IDENTIFIER` - Used for MQTT topics, mDNS hostname, OTA name
- Module enable flags: `USE_WIFI`, `USE_MQTT`, `USE_MDNS`, `USE_OTA`, `USE_ESPNOW`
- Network credentials, broker settings, pin definitions

### Module System

Five conditionally-compiled modules in `src/modules/`:

| Module | Flag | Requires | Purpose |
|--------|------|----------|---------|
| WiFi | `USE_WIFI` | - | Network connectivity with auto-reconnect |
| MQTT | `USE_MQTT` | WiFi | Pub/sub messaging, auto-status publishing |
| mDNS | `USE_MDNS` | WiFi | Local discovery (`hostname.local`) |
| OTA | `USE_OTA` | WiFi | Over-the-air firmware updates |
| ESP-NOW | `USE_ESPNOW` | - | Low-latency peer-to-peer (host/client modes) |

Dependency violations produce compile-time errors.

### Dual-Core Task Distribution

- **Core 0**: WiFi reconnection task (5s interval), OTA update handling
- **Core 1**: MQTT client loop, game logic, callbacks

### Code Flow

```
main.cpp          → setup() and loop() entry points, callback handlers
  ├── setup.cpp   → Module initialization, startup diagnostics
  ├── loop.cpp    → Main loop: MQTT updates, OTA checks, game logic
  └── RadioDial.cpp → Radio dial potentiometer reading
```

### MQTT Topic Convention

Topics auto-constructed from device identifier:
- Base: `{MQTT_TOPIC_PREFIX}{DEVICE_IDENTIFIER}` (e.g., `SP/EngineStatus`)
- Status: `{base}/status` (publishes "Online" on connect)
- Command: `{base}/cmd` (auto-subscribed)
- Absolute topics start with `/` to bypass prefix

### ESP-NOW Modes

- **Host** (`ESPNOW_HOST 1`): Broadcasts to all registered clients
- **Client** (`ESPNOW_HOST 0`): Listens via receive callbacks, sends to host MAC only

### Radio Dial Module

Reads a potentiometer on GPIO34 and maps it to discrete dial positions with dead zones.

**Configuration** (`config.h`):
- `POT_PIN 34` - ADC input pin
- `NUM_POSITIONS 5` - Number of dial stations (1-5)
- `NUM_SAMPLES 16` - Readings averaged for smoothing
- `HYSTERESIS 15` - Raw value change threshold
- `POS_HYSTERESIS 50` - Extra margin to prevent position flutter

**API** (`RadioDial.h`):
- `radioDialInit()` - Initialize (called in setup)
- `radioDialUpdate()` - Poll and print changes (called in loop)
- `radioDialGetPosition()` - Returns 0 (dead zone) or 1-5 (station)

**Wiring** (B100K potentiometer):
- Blue → 3.3V
- Green → GPIO34
- Yellow → GND

## Key Patterns

**MQTT Publish**: `mqttPublish("state", "SOLVED", true)` (topic suffix, message, retained)

**Callbacks**: Registered in `setup.cpp` via function pointers (`onMqttMessage`, `onEspNowReceive`)

**OTA Blocking**: Main loop checks `otaIsUpdating()` and returns early during updates

## Development Workflow

1. Edit `config.h` to set device name and enable required modules
2. Implement puzzle logic in `RadioDial.cpp` or add new modules
3. Build and upload with `pio run --target upload`

## Reference

See `template_use.md` for comprehensive module documentation with code examples.
