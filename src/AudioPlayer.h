#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <Arduino.h>

void audioPlayerInit();
void audioPlayerPlayTrack(uint16_t track);
void audioPlayerStop();
void audioPlayerPause();
void audioPlayerResume();
void audioPlayerSetVolume(uint8_t volume);

#endif
