#include <Arduino.h>
#include "pins.h"
#include "ihm.h"
#include "sensors.h"
#include "actuators.h"
#include "motion.h"

#define PAMI_WAIT 0
#define PAMI_RUN 1
#define PAMI_END 2

#define TIME_START_PAMI 0
#define TIME_END_PAMI 10000

#define MATCH_TIME 100000

long elapsedTime  = 0;
long startTime    = 0;
int  statePAMI = PAMI_WAIT;

void waitStart();
void updateMatchTime();
void testMatch();

void setup() {

  initIHM();
  initSensor();
  initMotion();
  initActuators();


  waitStart();
}

void loop() 
{
  //updateMatchTime();
  //testAntenne();
  testSensor();
  //testMatch();
  /*
  enableMotors();
  go(100);
  turn(90);
  turn(-90);
  go(-100);*/
}

void waitStart(){
  //Attendre que la tirette soit insérée
  debug("Insert Tirette");
  while(getTirette()) {
    delay(250); 
    checkColorTeam();
  }
  //Attendre que la tirette soit retirée
  debug("Wait Start");
  while(!getTirette()) {
    delay(250); 
    checkColorTeam();
  }
  debug("Start Robot");
  startTime = millis();
}

void updateMatchTime(){
  elapsedTime = millis() - startTime ;
  if(elapsedTime>= TIME_START_PAMI)   statePAMI = true;
  if(elapsedTime>= TIME_END_PAMI)  statePAMI = false;
}

void testMatch(){
  if(statePAMI == PAMI_RUN){
    enableMotors();
    // ToDo - Ecrire la strat
  }
  else if (statePAMI == PAMI_END){
    disableMotors(); // Desactive les moteurs
    while(1); // Fin de match
  }
  else {
    disableMotors(); // Desactive les moteurs
  }  
}