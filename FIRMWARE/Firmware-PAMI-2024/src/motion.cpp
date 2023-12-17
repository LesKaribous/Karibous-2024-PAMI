// motion.h
#include "motion.h"

// Position absolue du robot
Pose robotPose = {0.0f, 0.0f, 0.0f};


StepMode currentStepMode = SIXTEENTH_STEP;
int stepMultiplier = 16;
float circumferenceMM = WHEEL_DIAMETER_MM * PI;

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
  setStepMode(SIXTEENTH_STEP);
  // Desactive les moteurs
  digitalWrite(EN,HIGH);
  // Configure les vitesses et accelerations
  setMaxSpeed();
  setAcceleration();
}

void setMaxSpeed(float _maxSpeed){
  stepperG.setMaxSpeed(_maxSpeed);
  stepperD.setMaxSpeed(_maxSpeed);
}

void setAcceleration(float _acceleration){
  stepperG.setAcceleration(_acceleration);
  stepperD.setAcceleration(_acceleration);
}

void setStepMode(StepMode mode) {
  currentStepMode = mode;
  switch(mode) {
    case EIGHTH_STEP:
      digitalWrite(MS1, LOW);
      digitalWrite(MS2, LOW);
      stepMultiplier = 8;
      break;
    case HALF_STEP:
      digitalWrite(MS1, LOW);
      digitalWrite(MS2, HIGH);
      stepMultiplier = 2;
      break;
    case QUARTER_STEP:
      digitalWrite(MS1, HIGH);
      digitalWrite(MS2, LOW);
      stepMultiplier = 4;
      break;
    case SIXTEENTH_STEP:
      digitalWrite(MS1, HIGH);
      digitalWrite(MS2, HIGH);
      stepMultiplier = 16;
      break;
  }
}

void enableMotors(){
  digitalWrite(EN,LOW);
}

void disableMotors(){
  digitalWrite(EN,HIGH);
}

void updateMotion(){
  stepperG.run();
  stepperD.run();
}

long convertDistToStep(float _dist) {
  float revolutions = _dist / circumferenceMM;
  return static_cast<long>(revolutions * STEPS_PER_REVOLUTION * stepMultiplier);
}