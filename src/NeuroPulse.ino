#include <Arduino.h>

#include "config.h"
#include "AudioVisual.h"
#include "HardwareInputs.h"
#include "DisplayUI.h"
#include "Diagnostics.h"

// --------------------------------------------------
// Global state
// --------------------------------------------------
SystemState currentState = STATE_IDLE;

// --------------------------------------------------
// Function declarations used in this file
// --------------------------------------------------
void runBinaryTest();
void runJoystickTest();

// --------------------------------------------------
// SETUP
// --------------------------------------------------
void setup() {
  Serial.begin(9600);

  initInputs();
  initAudioVisual();

  if (!initDisplay()) {
    while (true) {
      playTone(400, 100);
      delay(200);
    }
  }

  randomSeed(analogRead(A3));

  drawIdleScreen(0);
}

// --------------------------------------------------
// Binary button test
// --------------------------------------------------
void runBinaryTest() {
  for (int roundNumber = 1;
       roundNumber <= TOTAL_BUTTON_ROUNDS;
       roundNumber++) {

    unsigned long waitTime =
      random(MIN_WAIT, MAX_WAIT + 1);

    unsigned long waitStart = millis();

    while (millis() - waitStart < waitTime) {
      if (pollButtonPress() != 0) {
        recordFalseStart();

        drawFalseStartScreen();
        playTone(200, 200);

        delay(800);
        return;
      }
    }

    int target = random(1, 3);

    drawTargetButton(target);
    playTone(1600, 100);

    unsigned long reactionStart = millis();
    int pressed = 0;

    while (
      millis() - reactionStart < BUTTON_TIMEOUT
    ) {
      pressed = pollButtonPress();

      if (pressed != 0) {
        break;
      }
    }

    unsigned long reactionTime =
      millis() - reactionStart;

    if (pressed == 0) {
      drawTimeoutScreen();
      playTone(280, 300);
    } else if (pressed == target) {
      int points;

      if (reactionTime < 300) {
        points = 100;
      } else {
        points = 70;
      }

      recordSuccess(reactionTime, points);
      drawSuccessScreen(reactionTime, points);
      playTone(1200, 150);
    } else {
      recordWrongChoice();
      drawWrongChoiceScreen(pressed, target);
      playTone(200, 300);
    }

    waitForButtonRelease(BUTTON_1);
    waitForButtonRelease(BUTTON_2);

    delay(600);
  }
}

// --------------------------------------------------
// Joystick visuomotor test
// --------------------------------------------------
void runJoystickTest() {
  for (
    int roundNumber = 1;
    roundNumber <= TOTAL_JOYSTICK_ROUNDS;
    roundNumber++
  ) {
    int direction = random(0, 4);

    drawArrowGraphic(direction);
    delay(500);

    unsigned long waitTime =
      random(MIN_WAIT, MAX_WAIT + 1);

    unsigned long waitStart = millis();
    bool falseStartDetected = false;

    while (millis() - waitStart < waitTime) {
      if (checkJoystickMovedEarly()) {
        falseStartDetected = true;
        break;
      }
    }

    if (falseStartDetected) {
      recordFalseStart();

      drawFalseStartScreen();
      playTone(200, 200);
      delay(800);

      // Wait until the joystick returns to its neutral position
      while (checkJoystickMovedEarly()) {
        delay(10);
      }

      // Skip only this round and begin the next one
      continue;
    }

    playTone(1750, 100);

    unsigned long reactionStart = millis();
    bool hit = false;

    while (
      millis() - reactionStart < JOYSTICK_TIMEOUT
    ) {
      if (isJoystickTriggered(direction)) {
        hit = true;
        break;
      }
    }

    unsigned long reactionTime =
      millis() - reactionStart;

    if (hit) {
      int points;

      if (reactionTime < 400) {
        points = 100;
      } else {
        points = 75;
      }

      recordSuccess(reactionTime, points);
      drawSuccessScreen(reactionTime, points);
      playTone(1400, 150);
    } else {
      drawTimeoutScreen();
      playTone(280, 300);
    }

    delay(600);
  }
}


// --------------------------------------------------
// MAIN LOOP
// --------------------------------------------------
void loop() {
  switch (currentState) {
    case STATE_IDLE:
      drawIdleScreen(0);

      if (checkStartPressed()) {
        waitForButtonRelease(START_BUTTON);
        currentState = STATE_START;
      }
      break;

    case STATE_START:
      resetDiagnostics();

      drawStartScreen();
      playTone(850, 150);
      ledDance();

      currentState = STATE_BUTTON_ROUND;
      break;

    case STATE_BUTTON_ROUND:
      runBinaryTest();

      currentState = STATE_JOYSTICK_ROUND;
      break;

    case STATE_JOYSTICK_ROUND:
      runJoystickTest();

      currentState = STATE_REPORT;
      break;

    case STATE_REPORT:
      renderReportPage1();
      delay(3500);

      renderReportPage2();
      delay(3500);

      renderReportPage3();
      victoryFanfare();
      delay(2000);

      drawIdleScreen(0);
      currentState = STATE_IDLE;
      break;
  }
}