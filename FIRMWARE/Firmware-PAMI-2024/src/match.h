// match.h
#ifndef MATCH_H
#define MATCH_H

#include <Arduino.h>
#include "ihm.h"

#define TIME_START_MATCH 4000
#define TIME_END_MATCH 14000

#define MATCH_WAIT 0
#define MATCH_BEGIN 1
#define PAMI_RUN 2
#define PAMI_STOP 3

int getMatchState();
void setMatchState(int _state);
void startMatch();
void updateMatchTime();

#endif // MATCH_H
