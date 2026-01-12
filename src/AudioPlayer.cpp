#include "AudioPlayer.h"
#include "config.h"
#include <DYPlayerArduino.h>

// Use Hardware Serial2 for DY-HV8F
static DY::Player player(&Serial2);

void audioPlayerInit() {
  Serial2.begin(9600, SERIAL_8N1, DYPLAYER_RX_PIN, DYPLAYER_TX_PIN);
  delay(100);  // Give module time to initialize

  player.setVolume(DYPLAYER_VOLUME);
  player.setCycleMode(DY::PlayMode::RepeatOne);  // Loop current track
  Serial.print("[Audio] Initialized, volume: ");
  Serial.print(DYPLAYER_VOLUME);
  Serial.println(", mode: RepeatOne");
}

void audioPlayerPlayTrack(uint16_t track) {
  player.playSpecified(track);
  delay(50);  // Let command process
  player.setCycleMode(DY::PlayMode::RepeatOne);  // Re-enable loop after track change
  Serial.print("[Audio] Playing track: ");
  Serial.println(track);
}

void audioPlayerStop() {
  player.stop();
  Serial.println("[Audio] Stopped");
}

void audioPlayerPause() {
  player.pause();
  Serial.println("[Audio] Paused");
}

void audioPlayerResume() {
  player.play();
  Serial.println("[Audio] Resumed");
}

void audioPlayerSetVolume(uint8_t volume) {
  player.setVolume(volume);
  Serial.print("[Audio] Volume set to: ");
  Serial.println(volume);
}
