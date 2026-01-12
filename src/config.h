#ifndef CONFIG_H
#define CONFIG_H

// ============================================================
//                    DEVICE IDENTIFIER
// ============================================================
#define DEVICE_IDENTIFIER "RadioReceiver"

// ============================================================
//                  ESP-NOW CONFIGURATION
// ============================================================
#define ESPNOW_HOST 0 // 1 = Host (broadcasts), 0 = Client (listens)
// Host MAC address - set this to your host device's MAC
// Only used when ESPNOW_HOST is 0 (client mode)
#define ESPNOW_HOST_MAC {0xA8, 0x42, 0xE3, 0xC8, 0x42, 0x78}

// ============================================================
//                     PIN DEFINITIONS
// ============================================================
#define POT_PIN 34 // Radio dial potentiometer (ADC1_CH6)

// DY-HV8F Audio Player (UART)
#define DYPLAYER_RX_PIN 16 // ESP32 RX <- DY-HV8F TX (IO0)
#define DYPLAYER_TX_PIN 17 // ESP32 TX -> DY-HV8F RX (IO1)

// ============================================================
//                   RADIO DIAL SETTINGS
// ============================================================
#define NUM_POSITIONS 5   // Number of dial positions/stations
#define NUM_SAMPLES 16    // ADC readings to average for smoothing
#define HYSTERESIS 15     // Ignore raw value changes smaller than this
#define POS_HYSTERESIS 50 // Extra margin to prevent position flutter

// ============================================================
//                   DY-HV8F AUDIO SETTINGS
// ============================================================
#define DYPLAYER_VOLUME 15     // Volume level (0-30)
#define STATIC_TRACK 1         // Track number for static/white noise
#define VERSIONS_PER_STATION 2 // Number of offset versions per station
#define DIAL_SETTLE_MS 200     // Wait time before switching audio

/*
 * SD CARD FILE STRUCTURE:
 * =======================
 * Files must be named 00001.mp3, 00002.mp3, etc.
 *
 * 00001.mp3 - Static/white noise (loops when between stations)
 * 00002.mp3 - Station 1, version A (different start offsets)
 * 00003.mp3 - Station 1, version B
 * 00004.mp3 - Station 2, version A
 * 00005.mp3 - Station 2, version B
 * 00006.mp3 - Station 3, version A
 * 00007.mp3 - Station 3, version B
 * 00008.mp3 - Station 4, version A
 * 00009.mp3 - Station 4, version B
 * 00010.mp3 - Station 5, version A
 * 00011.mp3 - Station 5, version B
 *
 * Formula: track = STATIC_TRACK + 1 + (station-1)*VERSIONS_PER_STATION + version
 * Example: Station 3, version B = 1 + 1 + (3-1)*2 + 1 = 7
 */

/*
 * DY-HV8F WIRING:
 * ===============
 * DY-HV8F Pin    ESP32 Pin       Notes
 * ----------     ---------       -----
 * VIN            VIN (5V)        Power supply (6-35VDC)
 * GND            GND             Common ground
 * IO0 (TX)       GPIO16 (RX2)    Serial data from player
 * IO1 (RX)       GPIO17 (TX2)    Serial data to player
 * SPK+           Speaker +       8ohm speaker
 * SPK-           Speaker -       8ohm speaker
 *
 * CON1           GND             \
 * CON2           GND              } One-line serial mode
 * CON3           3.3V            /
 *
 * Note: ESP32 is 3.3V, DY-HV8F is 5V tolerant on IO pins.
 *       No resistors needed for ESP32 connection.
 */

#endif
