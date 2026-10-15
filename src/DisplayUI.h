    

#ifndef DISPLAY_UI_H
#define DISPLAY_UI_H

#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;

bool initDisplay();

void drawIdleScreen(int messageIndex);
void drawStartScreen();

void drawTargetButton(int target);
void drawArrowGraphic(int direction);

void drawSuccessScreen(
  unsigned long reactionTime,
  int points
);

void drawWrongChoiceScreen(
  int pressed,
  int target
);

void drawFalseStartScreen();
void drawTimeoutScreen();

void renderReportPage1();
void renderReportPage2();
void renderReportPage3();

#endif