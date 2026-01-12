#include "RadioDial.h"
#include "config.h"
#include "AudioPlayer.h"

static int lastPosition = -1;
static int lastRawValue = 0;
static int currentPosition = -1;
static unsigned long positionChangeTime = 0;
static bool audioNeedsUpdate = false;

// Track which version to play next for each station (0=A, 1=B cycling)
static uint8_t stationVersion[NUM_POSITIONS] = {0};

// Station 5 enable control - starts disabled, enabled via ESP-NOW "Button On"
static bool Station_5_enable = false;

static int readSmoothed() {
  long total = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    total += analogRead(POT_PIN);
    delayMicroseconds(100);
  }
  return total / NUM_SAMPLES;
}

static uint16_t getTrackForStation(int station) {
  if (station == 0) {
    return STATIC_TRACK;
  }

  // Station 5 plays static unless enabled
  if (station == 5 && !Station_5_enable) {
    return STATIC_TRACK;
  }

  // Get current version and advance to next
  uint8_t version = stationVersion[station - 1];
  stationVersion[station - 1] = (version + 1) % VERSIONS_PER_STATION;

  // Calculate track number
  return STATIC_TRACK + 1 + (station - 1) * VERSIONS_PER_STATION + version;
}

void radioDialInit() {
  lastRawValue = readSmoothed();
  lastPosition = -1;
  currentPosition = -1;
}

int radioDialGetPosition() {
  return currentPosition;
}

void radioDialUpdate() {
  // Read averaged analog value
  int rawValue = readSmoothed();

  // Apply hysteresis - only accept change if it's significant
  if (abs(rawValue - lastRawValue) < HYSTERESIS) {
    rawValue = lastRawValue;
  } else {
    lastRawValue = rawValue;
  }

  // Calculate position with dead zones
  int zoneWidth = 4096 / NUM_POSITIONS;
  int zone = rawValue / zoneWidth;
  int posInZone = rawValue % zoneWidth;
  int deadZone = zoneWidth / 4;

  int position;
  if (zone >= NUM_POSITIONS) {
    position = NUM_POSITIONS;
  } else {
    if (posInZone >= deadZone && posInZone < (zoneWidth - deadZone)) {
      position = zone + 1;
    } else {
      position = 0;
    }
  }

  // Apply position hysteresis to prevent flutter at zone boundaries
  if (lastPosition >= 0 && position != lastPosition) {
    int margin;
    if (lastPosition == 0) {
      margin = deadZone + POS_HYSTERESIS;
      if (!(posInZone >= margin && posInZone < (zoneWidth - margin))) {
        position = 0;
      }
    } else {
      int activeStart = deadZone;
      int activeEnd = zoneWidth - deadZone;
      if (posInZone >= (activeStart - POS_HYSTERESIS) && posInZone < (activeEnd + POS_HYSTERESIS)) {
        position = lastPosition;
      }
    }
  }

  // Detect position change - start settle timer
  if (position != lastPosition) {
    positionChangeTime = millis();
    audioNeedsUpdate = true;
    lastPosition = position;
  }

  // After dial settles, update audio
  if (audioNeedsUpdate && (millis() - positionChangeTime >= DIAL_SETTLE_MS)) {
    audioNeedsUpdate = false;

    if (lastPosition != currentPosition) {
      currentPosition = lastPosition;

      uint16_t track = getTrackForStation(currentPosition);

      Serial.print("Position: ");
      Serial.print(currentPosition);
      Serial.print(" -> Track ");
      Serial.println(track);

      audioPlayerPlayTrack(track);
    }
  }
}

void radioDialSetStation5Enable(bool enable) {
  if (Station_5_enable != enable) {
    Station_5_enable = enable;
    Serial.print("[RadioDial] Station 5 ");
    Serial.println(enable ? "ENABLED" : "DISABLED");

    // If currently on station 5, force audio update
    if (currentPosition == 5) {
      audioNeedsUpdate = true;
      positionChangeTime = millis();
    }
  }
}

bool radioDialGetStation5Enable() {
  return Station_5_enable;
}
