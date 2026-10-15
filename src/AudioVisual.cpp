#include <Arduino.h>

#include "AudioVisual.h"
#include "DisplayUI.h"
#include "config.h"

const int leds[] = { LED_1, LED_2, LED_3, LED_4 };

static unsigned long idleLastAction = 0;
static unsigned long idleMessageTimer = 0;
static int arcadePattern = 0;
static int arcadeStep = 0;
static bool arcadePatternActive = false;
static int arcadeMessage = 0;
const unsigned long IDLE_STEP_TIME = 180;

void initAudioVisual() {
  for (int i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
  pinMode(BUZZER, OUTPUT);
}

void allLEDsOff() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(leds[i], LOW);
  }
  noTone(BUZZER);
}

void playTone(int frequency, int duration) {
  tone(BUZZER, frequency, duration);
}

void ledDance() {
  allLEDsOff();
  int forwardTones[] = { 523, 659, 784, 1046 };
  for (int i = 0; i < 4; i++) {
    digitalWrite(leds[i], HIGH);
    tone(BUZZER, forwardTones[i], 80);
    delay(90);
    digitalWrite(leds[i], LOW);
    tone(BUZZER, forwardTones[i] + 100, 25);
    delay(30);
  }

  int reverseTones[] = { 1175, 988, 784, 587 };
  for (int i = 3; i >= 0; i--) {
    digitalWrite(leds[i], HIGH);
    tone(BUZZER, reverseTones[3 - i], 80);
    delay(90);
    digitalWrite(leds[i], LOW);
    tone(BUZZER, reverseTones[3 - i] - 100, 25);
    delay(30);
  }

  for (int cycle = 0; cycle < 3; cycle++) {
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_3, HIGH);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_4, LOW);
    tone(BUZZER, 1400 + (cycle * 200), 50);
    delay(70);

    digitalWrite(LED_1, LOW);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_4, HIGH);
    tone(BUZZER, 1800 + (cycle * 200), 50);
    delay(70);
  }

  for (int i = 0; i < 4; i++) digitalWrite(leds[i], HIGH);
  tone(BUZZER, 2093, 200);
  delay(220);
  allLEDsOff();
}

void victoryFanfare() {
  int fanfareNotes[] = { 523, 659, 784, 1046, 1318, 1568, 1760, 2093 };

  for (int i = 0; i < 8; i++) {
    allLEDsOff();
    digitalWrite(leds[i % 4], HIGH);
    tone(BUZZER, fanfareNotes[i], 70);
    delay(85);
  }

  for (int b = 0; b < 3; b++) {
    for (int i = 0; i < 4; i++) digitalWrite(leds[i], HIGH);
    tone(BUZZER, 1760, 60);
    delay(80);

    allLEDsOff();
    tone(BUZZER, 880, 30);
    delay(50);
  }

  for (int i = 0; i < 4; i++) digitalWrite(leds[i], HIGH);
  tone(BUZZER, 2093, 350);
  delay(400);
  allLEDsOff();
}

void triggerErrorBuzzer() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) digitalWrite(leds[j], HIGH);
    tone(BUZZER, 240, 60);
    delay(70);
    allLEDsOff();
    tone(BUZZER, 160, 40);
    delay(70);
  }
}

void triggerEarlyBuzzer() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, LOW);
    tone(BUZZER, 260, 60);
    delay(70);

    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, HIGH);
    digitalWrite(LED_4, HIGH);
    tone(BUZZER, 190, 60);
    delay(70);
  }
  allLEDsOff();
}

static void runIdlePatternStep() {
  switch (arcadePattern) {
    case 0:
      allLEDsOff();
      if (arcadeStep < 4) {
        digitalWrite(leds[arcadeStep], HIGH);
        tone(BUZZER, 700 + (arcadeStep * 180), 30);
      }
      arcadeStep++;
      if (arcadeStep >= 8) { arcadePatternActive = false; allLEDsOff(); }
      break;

    case 1:
      allLEDsOff();
      if (arcadeStep == 0) {
        digitalWrite(LED_1, HIGH); digitalWrite(LED_4, HIGH); tone(BUZZER, 800, 35);
      } else if (arcadeStep == 1) {
        digitalWrite(LED_2, HIGH); digitalWrite(LED_3, HIGH); tone(BUZZER, 1200, 35);
      } else if (arcadeStep == 2) {
        digitalWrite(LED_1, HIGH); digitalWrite(LED_4, HIGH); tone(BUZZER, 800, 35);
      } else {
        allLEDsOff(); arcadePatternActive = false; return;
      }
      arcadeStep++;
      break;

    case 2:
      allLEDsOff();
      if (arcadeStep % 2 == 0) {
        digitalWrite(LED_1, HIGH); digitalWrite(LED_3, HIGH); tone(BUZZER, 900, 30);
      } else {
        digitalWrite(LED_2, HIGH); digitalWrite(LED_4, HIGH); tone(BUZZER, 1300, 30);
      }
      arcadeStep++;
      if (arcadeStep >= 6) { arcadePatternActive = false; allLEDsOff(); }
      break;

    case 3: {
      allLEDsOff();
      int picked = random(0, 4);
      digitalWrite(leds[picked], HIGH);
      tone(BUZZER, 950 + (picked * 220), 35);
      arcadeStep++;
      if (arcadeStep >= 6) { arcadePatternActive = false; allLEDsOff(); }
      break;
    }

    case 4:
      allLEDsOff();
      if (arcadeStep % 2 == 0) {
        digitalWrite(LED_2, HIGH); digitalWrite(LED_3, HIGH); tone(BUZZER, 1050, 35);
      } else {
        digitalWrite(LED_1, HIGH); digitalWrite(LED_4, HIGH); tone(BUZZER, 1450, 35);
      }
      arcadeStep++;
      if (arcadeStep >= 6) { arcadePatternActive = false; allLEDsOff(); }
      break;

    case 5:
      if (arcadeStep % 2 == 0) {
        for (int i = 0; i < 4; i++) digitalWrite(leds[i], HIGH);
        tone(BUZZER, 1900, 60);
      } else {
        allLEDsOff(); tone(BUZZER, 1200, 40);
      }
      arcadeStep++;
      if (arcadeStep >= 8) { allLEDsOff(); arcadePatternActive = false; }
      break;

    case 6:
      allLEDsOff();
      if (arcadeStep < 4) {
        digitalWrite(leds[arcadeStep], HIGH);
        tone(BUZZER, 1500 - (arcadeStep * 220), 30);
      }
      arcadeStep++;
      if (arcadeStep >= 4) { arcadeStep = 0; arcadePatternActive = false; allLEDsOff(); }
      break;
  }
}

void runIdleAnimation() {
  unsigned long now = millis();

  if (now - idleMessageTimer >= 3500) {
    idleMessageTimer = now;
    arcadeMessage = (arcadeMessage + 1) % 5;
    drawIdleScreen(arcadeMessage);
  }

  if (!arcadePatternActive) {
    int chance = random(0, 100);
    arcadePattern = (chance < 7) ? 5 : random(0, 7);
    if (arcadePattern == 5 && chance >= 7) arcadePattern = random(0, 5);
    arcadeStep = 0;
    arcadePatternActive = true;
    idleLastAction = now;
  }

  if (now - idleLastAction >= IDLE_STEP_TIME) {
    idleLastAction = now;
    runIdlePatternStep();
  }
}