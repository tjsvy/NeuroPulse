#ifndef AUDIO_VISUAL_H
#define AUDIO_VISUAL_H

#include "Config.h"

extern const int leds[];

void initAudioVisual();
void allLEDsOff();
void ledDance();
void victoryFanfare();
void playTone(int frequency, int duration);
void triggerErrorBuzzer();
void triggerEarlyBuzzer();
void runIdleAnimation();

#endif // AUDIO_VISUAL_H