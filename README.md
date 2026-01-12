# Red Rider Radio Dial

ESP32-S3 radio dial prop for the Christmas Red Rider escape room. Players tune a physical dial to find secret frequencies, triggering themed audio.

## Hardware

- **Board**: ESP32-S3-DevKitC-1 (N8R2: 8MB Flash, 2MB PSRAM)
- **Audio**: DY-HV8F MP3 player module
- **Input**: B100K potentiometer (radio dial)

## Features

- **Radio Dial**: 5 tunable stations with static between frequencies
- **Audio Playback**: Station-specific audio with random version selection
- **ESP-NOW**: Receives enable/disable from room hub

## Quick Start

```bash
pio run                    # Build
pio run --target upload    # Upload via USB
pio device monitor         # Serial output (115200 baud)
```

## Wiring

### Potentiometer (B100K)
| Wire | Connection |
|------|------------|
| Blue | 3.3V |
| Green | GPIO34 (ADC) |
| Yellow | GND |

### DY-HV8F Audio
| DY-HV8F | ESP32-S3 |
|---------|----------|
| VIN | 5V |
| GND | GND |
| IO0 (TX) | GPIO16 |
| IO1 (RX) | GPIO17 |

## SD Card Structure

```
00001.mp3  - Static/white noise
00002.mp3  - Station 1, version A
00003.mp3  - Station 1, version B
00004.mp3  - Station 2, version A
...
```

## Configuration

Edit `src/config.h` to adjust dial positions, volume, and pin assignments.

## Part of Otherworld Escape Rooms

This prop is part of the [OER](https://github.com/TenMoreMinutesProductions) escape room system.
