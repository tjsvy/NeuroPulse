#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// -------------------- PINS --------------------

#define START_BUTTON 12

#define BUTTON_1 4
#define BUTTON_2 5

#define LED_1 8
#define LED_2 9
#define LED_3 10
#define LED_4 11

#define BUZZER 7

#define JOY_X A0
#define JOY_Y A1
#define JOY_SW A2

// -------------------- OLED --------------------

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

// -------------------- TEST SETTINGS --------------------

#define TOTAL_BUTTON_ROUNDS 2
#define TOTAL_JOYSTICK_ROUNDS 3

#define WRONG_BUTTON_PENALTY 25
#define EARLY_PENALTY 25

#define MIN_WAIT 1500UL
#define MAX_WAIT 4000UL

#define BUTTON_TIMEOUT 3000UL
#define JOYSTICK_TIMEOUT 3000UL

// -------------------- STATE MACHINE --------------------

enum SystemState {
  STATE_IDLE,
  STATE_START,
  STATE_BUTTON_ROUND,
  STATE_JOYSTICK_ROUND,
  STATE_REPORT
};

extern SystemState currentState;

#endif