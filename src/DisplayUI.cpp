#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "config.h"
#include "DisplayUI.h"
#include "Diagnostics.h"

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

bool initDisplay() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    return false;
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.display();

  return true;
}

void drawIdleScreen(int messageIndex) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(10, 7);
  display.println(F("PVT DIAGNOSTIC"));

  display.drawLine(10, 19, 118, 19, SSD1306_WHITE);

  display.setCursor(8, 30);
  display.println(F("PRESS START TO TEST"));

  display.setTextSize(2);
  display.setCursor(34, 46);
  display.println(F("STANDBY"));

  display.display();
}

void drawStartScreen() {
  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(20, 12);
  display.println(F("SUBJECT"));

  display.setCursor(32, 36);
  display.println(F("READY"));

  display.display();
}

void drawTargetButton(int target) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(47, 2);
  display.println(F("PRESS"));

  display.setTextSize(4);
  display.setCursor(52, 17);

  if (target == 1) {
    display.println(F("1"));
  } else {
    display.println(F("2"));
  }

  display.display();
}

void drawArrowGraphic(int direction) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(50, 2);
  display.println(F("AXIS"));

  switch (direction) {
    case 0:
      display.fillTriangle(
        64, 16,
        44, 34,
        84, 34,
        SSD1306_WHITE
      );
      display.fillRect(58, 34, 13, 20, SSD1306_WHITE);
      break;

    case 1:
      display.fillRect(58, 16, 13, 20, SSD1306_WHITE);
      display.fillTriangle(
        64, 56,
        44, 36,
        84, 36,
        SSD1306_WHITE
      );
      break;

    case 2:
      display.fillTriangle(
        42, 36,
        62, 18,
        62, 54,
        SSD1306_WHITE
      );
      display.fillRect(62, 30, 24, 13, SSD1306_WHITE);
      break;

    case 3:
      display.fillRect(42, 30, 24, 13, SSD1306_WHITE);
      display.fillTriangle(
        86, 36,
        66, 18,
        66, 54,
        SSD1306_WHITE
      );
      break;
  }

  display.display();
}

void drawSuccessScreen(
  unsigned long reactionTime,
  int points
) {
  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(35, 3);
  display.println(F("NICE!"));

  display.setTextSize(1);
  display.setCursor(35, 29);
  display.print(reactionTime);
  display.println(F(" ms"));

  display.setCursor(25, 43);
  display.print(F("+"));
  display.print(points);
  display.println(F(" PTS"));

  display.display();
}

void drawWrongChoiceScreen(
  int pressed,
  int target
) {
  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(34, 4);
  display.println(F("ERROR!"));

  display.setTextSize(1);

  display.setCursor(20, 29);
  display.print(F("HIT: "));
  display.println(pressed);

  display.setCursor(20, 41);
  display.print(F("REQ: "));
  display.println(target);

  display.display();
}

void drawFalseStartScreen() {
  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(12, 10);
  display.println(F("INHIBITION"));

  display.setTextSize(1);
  display.setCursor(20, 34);
  display.println(F("FALSE MOTOR TRIGGER"));

  display.display();
}

void drawTimeoutScreen() {
  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(22, 10);
  display.println(F("TIMEOUT"));

  display.setTextSize(1);
  display.setCursor(28, 40);
  display.println(F("LATENCY EXCEEDED"));

  display.display();
}

void renderReportPage1() {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(8, 2);
  display.println(F("PSYCHOMOTOR REPORT"));

  display.drawLine(8, 12, 120, 12, SSD1306_WHITE);

  display.setCursor(8, 18);
  display.print(F("MEAN CHOICE: "));
  display.print(currentSession.getMeanRT());
  display.println(F(" ms"));

  display.setCursor(8, 30);
  display.print(F("COMPLETED: "));
  display.print(currentSession.validRounds);
  display.println(F("/5"));

  display.setCursor(8, 42);
  display.print(F("FALSE MOTOR: "));
  display.println(currentSession.falseStarts);

  display.setCursor(8, 54);
  display.print(F("WRONG CHOICE: "));
  display.println(currentSession.wrongChoices);

  display.display();
}

void renderReportPage2() {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(12, 2);
  display.println(F("DRIVER FITNESS EVAL"));

  display.drawLine(12, 12, 116, 12, SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(12, 18);
  display.println(currentSession.getDriverTierStr());

  display.setTextSize(1);
  display.setCursor(12, 38);
  display.print(F("COG AGE: "));
  display.println(currentSession.getCognitiveAgeStr());

  display.setCursor(12, 50);
  display.print(F("SCORE: "));
  display.print(currentSession.score);
  display.println(F(" / 500"));

  display.display();
}

void renderReportPage3() {
  float distance = currentSession.getHighwayLagDistance();

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(10, 2);
  display.println(F("KINEMATIC BRAKE LAG"));

  display.drawLine(10, 12, 118, 12, SSD1306_WHITE);

  display.setCursor(10, 18);
  display.println(F("AT 100 KM/H SPEED:"));

  display.setTextSize(2);
  display.setCursor(14, 30);
  display.print(distance, 1);
  display.println(F(" M"));

  display.setTextSize(1);
  display.setCursor(10, 52);
  display.println(currentSession.getKinematicRiskStr());

  display.display();
}