// motion.h
#include "motion.h"

// Position absolue du robot
float   x = 0, 
        y = 0,
        rot = 0;

// Position en nombre de steps des moteurs
long posG = 0,
     posD = 0;

// Commande à atteindre pour chaque moteur
long cmdG = 0,
     cmdD = 0;

AccelStepper stepperG(AccelStepper::DRIVER, STEP_X, DIR_X);
AccelStepper stepperD(AccelStepper::DRIVER, STEP_Y, DIR_Y);

void initMotion(){
  // Configure les pins
  pinMode(EN,OUTPUT);
  pinMode(DIR_X,OUTPUT);
  pinMode(STEP_X,OUTPUT);
  pinMode(DIR_Y,OUTPUT);
  pinMode(STEP_Y,OUTPUT);

  pinMode(MS1,OUTPUT);
  pinMode(MS2,OUTPUT);
  // Configure les pas
  digitalWrite(MS1,HIGH);
  digitalWrite(MS2,HIGH);
  // Desactive les moteurs
  digitalWrite(EN,HIGH);
  // Configure le moteur gauche
  stepperG.setMaxSpeed(10000.0);
  stepperG.setAcceleration(5000.0);
  // Configure le moteur droit
  stepperD.setMaxSpeed(10000.0);
  stepperD.setAcceleration(5000.0);
}

void setCoordinate(float _x, float _y, float _rot){
  x = _x;
  y = _y;
  rot = _rot;
}

void updateMotors(){
  stepperG.run();
  stepperD.run();
}

void enableMotors(){
  digitalWrite(EN,LOW);
}

void disableMotors(){
  digitalWrite(EN,HIGH);
}
