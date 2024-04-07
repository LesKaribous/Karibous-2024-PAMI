// 
#include "match.h"

long elapsedTime  = 0;
long startTime    = 0;

int matchState = MATCH_WAIT;

int getMatchState(){
    return matchState;
}
void setMatchState(int _state){
    matchState = _state;
}
void startMatch(){
    startTime = millis();
    matchState = MATCH_BEGIN;
}
void updateMatchTime(){
  if(getMatchState() == MATCH_BEGIN || getMatchState() == PAMI_RUN){
    elapsedTime = millis() - startTime ;
    if(elapsedTime>= TIME_END_MATCH)
    {
        setMatchState(PAMI_STOP);
        debug("Match End");
    }
    else if(elapsedTime>= TIME_START_MATCH && getMatchState() == MATCH_BEGIN)
    {
        setMatchState(PAMI_RUN);
        debug("Match Run");
    }
  }
}