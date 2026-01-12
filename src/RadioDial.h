#ifndef RADIODIAL_H
#define RADIODIAL_H

#include <Arduino.h>

void radioDialInit();
void radioDialUpdate();
int radioDialGetPosition();
void radioDialSetStation5Enable(bool enable);
bool radioDialGetStation5Enable();

#endif
