#include <Arduino.h>

#include "config.h"
#include "Diagnostics.h"

TestSession currentSession;

void TestSession::reset() {
  score = 0;
  totalReactionTime = 0;
  validRounds = 0;
  falseStarts = 0;
  wrongChoices = 0;
}

unsigned long TestSession::getMeanRT() const {
  if (validRounds == 0) {
    return 999;
  }

  return totalReactionTime / validRounds;
}

float TestSession::getHighwayLagDistance() const {
  const float speedMetersPerSecond = 27.78f;

  return (getMeanRT() * speedMetersPerSecond) / 1000.0f;
}

const __FlashStringHelper* TestSession::getCognitiveAgeStr() const {
  unsigned long rt = getMeanRT();

  if (rt < 380) {
    return F("18-25 YRS");
  } else if (rt < 480) {
    return F("26-40 YRS");
  } else if (rt < 620) {
    return F("41-60 YRS");
  } else {
    return F("65+ / SLOW");
  }
}

const __FlashStringHelper* TestSession::getDriverTierStr() const {
  unsigned long rt = getMeanRT();

  if (rt < 380 && falseStarts == 0 && wrongChoices == 0) {
    return F("FIT: TIER A");
  } else if (rt < 480 && falseStarts <= 1) {
    return F("FIT: TIER B");
  } else if (rt < 620) {
    return F("MILD FATIGUE");
  } else {
    return F("HIGH RISK!");
  }
}

const __FlashStringHelper* TestSession::getKinematicRiskStr() const {
  float distance = getHighwayLagDistance();

  if (distance <= 10.5f) {
    return F("SAFE REACTION GAP");
  } else if (distance <= 16.0f) {
    return F("NORMAL PASSENGER GAP");
  } else {
    return F("CRITICAL CRASH HAZARD");
  }
}
void resetDiagnostics() {
  currentSession.reset();
}

void recordSuccess(
  unsigned long reactionTime,
  int points
) {
  currentSession.totalReactionTime += reactionTime;
  currentSession.validRounds++;
  currentSession.score += points;
}

void recordFalseStart() {
  currentSession.falseStarts++;

  currentSession.score -= EARLY_PENALTY;

  if (currentSession.score < 0) {
    currentSession.score = 0;
  }
}

void recordWrongChoice() {
  currentSession.wrongChoices++;

  currentSession.score -= WRONG_BUTTON_PENALTY;

  if (currentSession.score < 0) {
    currentSession.score = 0;
  }
}