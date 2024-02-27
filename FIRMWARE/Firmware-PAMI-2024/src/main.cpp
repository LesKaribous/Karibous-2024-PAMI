#include <Arduino.h>
#include "pins.h"
#include "ihm.h"
#include "sensors.h"
#include "actuators.h"
#include "motion.h"

#define PAMI_WAIT 0
#define PAMI_RUN 1
#define PAMI_END 2

#define TIME_START_PAMI 4000
#define TIME_END_PAMI 100000

#define MATCH_TIME 100000

//TaskHandle_t Task1;

long elapsedTime  = 0;
long startTime    = 4000;
int  statePAMI = PAMI_WAIT;


void waitStart();
void updateMatchTime();
void datumPosition(int robotNumber, int teamColor);
void match();
void strategiePAMI();

void setup() {

  initIHM();
  initSensor();
  initMotion();
  initActuators();

  drawSplashScreen();
  pairingScreen();
  drawBackLcd();

  enableMotors();
  antennasUp();

  waitStart();
}

void loop() 
{
  updateMatchTime();
  match();
}

void waitStart(){
  //Attendre que la tirette n'est soit plus présente
  infoLCD("Remove Tirette");
  while(getTirette()) {
    delay(250); 
    checkColorTeam();
  }
  //Attendre que la tirette soit insérée
  infoLCD("Insert Tirette");
  while(!getTirette() && getRobotState() != READY) {
    delay(250); 
    checkColorTeam();
  }
  // Si la tirette a été insérée
  // Le robot va être lancés avec la tirette
  // Les message de démarrage seront envoyé sur le réseau
  // Seul le PAMI 1 envoi les messages

  delay(2000);
  datumPosition(getRobotNumber(), getTeamColor());


  if(getTirette()){
    BroadcastMessage(ARMED);
    setRobotState(READY);
    infoLCD("Wait Start");
    //Attendre que la tirette soit retirée
    while(getTirette()) delay(250);

    BroadcastMessage(START_MATCH);
    setRobotState(MATCH_STARTED);
  }
  else
  {
    // Cas des PAMIS 2 et 3 qui attendent le lancement du match sans tirette
    infoLCD("Wait Start");
    //Attendre l'ordre du PAMI 1
    while(getRobotState() != MATCH_STARTED) delay(250);
  }
  infoLCD("Start Robot");
  // Démarrage du compteur !
  startTime = millis();
}

void updateMatchTime(){
  elapsedTime = millis() - startTime ;
  if(statePAMI != PAMI_END){
    if(elapsedTime>= TIME_END_PAMI)         statePAMI = PAMI_END;
    else if(elapsedTime>= TIME_START_PAMI)  statePAMI = PAMI_RUN;
  }
}

void datumPosition(int robotNumber, int teamColor){

  // Datum at low Speed
  setMaxSpeed(DATUM_SPEED);
  setAcceleration(DATUM_ACCELERATION);
  // Datum Y
  go(-100);
  // Save Y position and orientation
  setCurrentY(CENTER_POSITION_MM);
  setCurrentRot(270);
  // Go to safe position
  goTo(0,80,270);

  if(robotNumber == 1){
    debug("datumPosition robot 1");
    if(teamColor == TEAM_BLUE){
      goTo(0,80,0);
      go(-100);
      // SaveX position and orientation
      setCurrentX(1050+CENTER_POSITION_MM);
      setCurrentRot(0);
      // Go to safe position
      goTo(1120,80,270);
    }
    else{
      // WIP
    }
  }
  else if(robotNumber == 2){
    debug("datumPosition robot 2");
    if(teamColor == TEAM_BLUE){
      goTo(0,80,0);
      go(-100);
      // SaveX position and orientation
      setCurrentX(1050+CENTER_POSITION_MM);
      setCurrentRot(0);
      // Go to safe position
      goTo(1120+130,80,270);
    }
  }
  else if(robotNumber == 3){
    debug("datumPosition robot 3");
    if(teamColor == TEAM_BLUE){
      goTo(0,100,0);
      go(-100);
      // SaveX position and orientation
      setCurrentX(1050+CENTER_POSITION_MM);
      setCurrentRot(0);
      // Go to safe position
      goTo(1120+260,80,270);
    }
  }
  else {
    debug("ERROR robot number");
  }

  setMaxSpeed(MAX_SPEED);
  setAcceleration(MAX_ACCELERATION);
}

void match(){
  if(statePAMI == PAMI_RUN){
    enableMotors();
    strategiePAMI();
    statePAMI = PAMI_END;
  }
  else if (statePAMI == PAMI_END){
    disableMotors(); // Desactive les moteurs
    while(1); // Fin de match
  }
  else {
    disableMotors(); // Desactive les moteurs
  }  
}

void strategiePAMI(){

  if(getRobotNumber() == 1){
    if(getTeamColor() == TEAM_BLUE){
      goTo(750,180);
      goTo(750,0);
      antennasDown();
    }
    else{
      // WIP
    }
  }
  else if(getRobotNumber() == 2){
    if(getTeamColor() == TEAM_BLUE){
      goTo(1200,300);
      goTo(400,300);
      antennasDown();
    }
    else{
      // WIP
    }
  }
  else if(getRobotNumber() == 3){
    if(getTeamColor() == TEAM_BLUE){
      goTo(1350,450);
      goTo(0,550);
      antennasDown();
    }
    else{
      // WIP
    }
  }
}