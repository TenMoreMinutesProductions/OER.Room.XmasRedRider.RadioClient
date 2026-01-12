#include <Arduino.h>
#include "setup.h"
#include "loop.h"
#include "RadioDial.h"

// ============================================================
//                   CALLBACK FUNCTIONS
// ============================================================

// Called when ESP-NOW message is received
void onEspNowReceive(const uint8_t* mac, const uint8_t* data, int len) {
  char msg[len + 1];
  memcpy(msg, data, len);
  msg[len] = '\0';

  if (strcmp(msg, "1") == 0 || strcmp(msg, "ON") == 0) {
    Serial.println("Button On");
    radioDialSetStation5Enable(true);
  } else if (strcmp(msg, "0") == 0 || strcmp(msg, "OFF") == 0) {
    Serial.println("Button Off");
    radioDialSetStation5Enable(false);
  } else {
    Serial.print("Button ");
    Serial.println(msg);
  }
}

// Called when ESP-NOW send completes
void onEspNowSend(const uint8_t* mac, bool success) {
  Serial.print("[ESP-NOW] Send ");
  Serial.println(success ? "success" : "failed");
}

// ============================================================
//                     ARDUINO ENTRY POINTS
// ============================================================
void setup() {
  setupInit();
}

void loop() {
  loopMain();
}
