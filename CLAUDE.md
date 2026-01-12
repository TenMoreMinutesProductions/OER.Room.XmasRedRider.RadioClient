# CLAUDE.md

## Project Overview

ESP32-S3 radio dial prop for the Christmas Red Rider room. Reads potentiometer for station tuning and plays audio via DY-HV8F.

**Target Board:** ESP32-S3-DevKitC-1 (N8R2: 8MB Flash, 2MB PSRAM)

## Build Commands

```bash
pio run                    # Build
pio run --target upload    # Upload to ESP32-S3
pio run --target clean     # Clean build
pio device monitor         # Serial monitor (115200 baud)
```

## Architecture

### Code Flow
```
main.cpp          → Entry points, ESP-NOW callbacks
├── setup.cpp     → Module initialization
├── loop.cpp      → Main loop dispatcher
├── RadioDial.cpp → Potentiometer reading, position detection
└── AudioPlayer.cpp → DY-HV8F control, track selection
```

### Key Modules
- **RadioDial**: ADC reading with hysteresis, maps to 5 stations
- **AudioPlayer**: DY-HV8F UART control, static/station switching
- **ESP-NOW**: Client mode - listens for hub commands

### Configuration
All settings in `src/config.h`:
- `DEVICE_IDENTIFIER` - Device name
- `POT_PIN`, `DYPLAYER_*` - Hardware pins
- `NUM_POSITIONS`, `HYSTERESIS` - Dial tuning
- `DYPLAYER_VOLUME` - Audio volume (0-30)

## ESP32-S3 Notes

- Verify ADC pin mapping for S3 variant
- PSRAM enabled for buffering
- Different GPIO numbering than ESP32
