

#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <Arduino.h>
#include "config.h"

struct TestSession {
  int score = 0;
  unsigned long totalReactionTime = 0;
  int validRounds = 0;
  int falseStarts = 0;
  int wrongChoices = 0;

  void reset();

  unsigned long getMeanRT() const;
  float getHighwayLagDistance() const;

  const __FlashStringHelper* getCognitiveAgeStr() const;
  const __FlashStringHelper* getDriverTierStr() const;
  const __FlashStringHelper* getKinematicRiskStr() const;
};

extern TestSession currentSession;

void resetDiagnostics();

void recordSuccess(
  unsigned long reactionTime,
  int points
);

void recordFalseStart();

void recordWrongChoice();

#endif