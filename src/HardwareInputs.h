

#ifndef HARDWARE_INPUTS_H
#define HARDWARE_INPUTS_H

void initInputs();
void initializeHardwareInputs();

bool isStartButtonPressed();
bool checkStartPressed();

void waitForButtonRelease(int pin);

int checkButtonPressed();
int pollButtonPress();

bool checkEarlyButtonPress();

bool checkEarlyJoystickMovement();
bool checkJoystickMovedEarly();

bool isJoystickDirectionCorrect(int direction);
bool isJoystickTriggered(int direction);

#endif