#include <Arduino.h>

#include "config.h"
#include "HardwareInputs.h"

// --------------------------------------------------
// Joystick calibration
// --------------------------------------------------
//
// Keep the joystick untouched and centered when the
// Arduino starts. The Arduino measures the actual
// center values automatically.
//
// These values are updated in initInputs().
//

int joystickCenterX = 512;
int joystickCenterY = 512;

const int JOYSTICK_DEADZONE = 120;
const int JOYSTICK_TRIGGER_MARGIN = 180;

// --------------------------------------------------
// Initialize buttons and joystick
// --------------------------------------------------
void initInputs() {
  pinMode(START_BUTTON, INPUT_PULLUP);
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(JOY_SW, INPUT_PULLUP);

  delay(300);

  // Read the joystick while it is centered
  long totalX = 0;
  long totalY = 0;

  for (int i = 0; i < 20; i++) {
    totalX += analogRead(JOY_X);
    totalY += analogRead(JOY_Y);
    delay(5);
  }

  joystickCenterX = totalX / 20;
  joystickCenterY = totalY / 20;
}

// Alias used by some versions of the program
void initializeHardwareInputs() {
  initInputs();
}

// --------------------------------------------------
// Start button
// --------------------------------------------------
bool isStartButtonPressed() {
  return digitalRead(START_BUTTON) == LOW;
}

bool checkStartPressed() {
  return isStartButtonPressed();
}

// --------------------------------------------------
// Wait for a button release
// --------------------------------------------------
void waitForButtonRelease(int pin) {
  while (digitalRead(pin) == LOW) {
    delay(5);
  }
}

// --------------------------------------------------
// Read binary buttons
//
// Returns:
// 0 = no button
// 1 = Button 1
// 2 = Button 2
// --------------------------------------------------
int checkButtonPressed() {
  if (digitalRead(BUTTON_1) == LOW) {
    return 1;
  }

  if (digitalRead(BUTTON_2) == LOW) {
    return 2;
  }

  return 0;
}

int pollButtonPress() {
  return checkButtonPressed();
}

// --------------------------------------------------
// Detect early button movement
// --------------------------------------------------
bool checkEarlyButtonPress() {
  return checkButtonPressed() != 0;
}

// --------------------------------------------------
// Determine whether joystick is outside its neutral zone
// --------------------------------------------------
bool checkEarlyJoystickMovement() {
  int x = analogRead(JOY_X);
  int y = analogRead(JOY_Y);

  bool xMoved =
    abs(x - joystickCenterX) > JOYSTICK_DEADZONE;

  bool yMoved =
    abs(y - joystickCenterY) > JOYSTICK_DEADZONE;

  return xMoved || yMoved;
}

bool checkJoystickMovedEarly() {
  return checkEarlyJoystickMovement();
}

// --------------------------------------------------
// Check joystick direction
//
// Direction values:
// 0 = UP
// 1 = DOWN
// 2 = LEFT
// 3 = RIGHT
//
// This version assumes:
// UP    = low Y
// DOWN  = high Y
// LEFT  = high X
// RIGHT = low X
//
// If one axis is reversed, only swap that axis below.
// --------------------------------------------------
bool isJoystickDirectionCorrect(int direction) {
  int x = analogRead(JOY_X);
  int y = analogRead(JOY_Y);

  switch (direction) {
    case 0:
      // UP
      return y < joystickCenterY - JOYSTICK_TRIGGER_MARGIN;

    case 1:
      // DOWN
      return y > joystickCenterY + JOYSTICK_TRIGGER_MARGIN;

    case 2:
      // LEFT
      return x > joystickCenterX + JOYSTICK_TRIGGER_MARGIN;

    case 3:
      // RIGHT
      return x < joystickCenterX - JOYSTICK_TRIGGER_MARGIN;
  }

  return false;
}

bool isJoystickTriggered(int direction) {
  return isJoystickDirectionCorrect(direction);
}