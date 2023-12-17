// motion.h
#include "motion.h"

// Position absolue du robot
Pose robotPose = {0.0f, 0.0f, 0.0f};

StepMode currentStepMode = SIXTEENTH_STEP;
int stepMultiplier = 16;
float circumferenceMM = WHEEL_DIAMETER_MM * PI;

// create the stepper motor object
Stepper motor_G(STEP_G, DIR_G);   //STEP, DIR
Stepper motor_D(STEP_D, DIR_D);   //STEP, DIR
StepControl motors;

void initMotion(){
  // Configure les pins
  pinMode(EN,OUTPUT);
  pinMode(DIR_G,OUTPUT);
  pinMode(STEP_G,OUTPUT);
  pinMode(DIR_D,OUTPUT);
  pinMode(STEP_D,OUTPUT);
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
   motor_G.setMaxSpeed(_maxSpeed);       // steps/s
   motor_D.setMaxSpeed(_maxSpeed);       // steps/s
}

void setAcceleration(float _acceleration){
   motor_G.setAcceleration(_acceleration); // steps/s^2 
   motor_D.setAcceleration(_acceleration); // steps/s^2 

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
  // NA
}

void go(float _dist){
  // WARNING : Blocking function
  long stepValue = convertDistToStep(_dist);
  motor_G.setTargetRel(-stepValue);
  motor_D.setTargetRel(stepValue);
  motors.move(motor_G, motor_D);
}

void turn(float _angle){
  // WARNING : Blocking function
  long stepValue = convertAngleToStep(_angle);
  motor_G.setTargetRel(stepValue);
  motor_D.setTargetRel(stepValue);
  motors.move(motor_G, motor_D);
}

long convertDistToStep(float _dist) {
  float revolutions = _dist / circumferenceMM;
  return static_cast<long>(revolutions * STEPS_PER_REVOLUTION * stepMultiplier);
}

// Convert angle to step for one wheel (robot turning on center)
long convertAngleToStep(float _angle) {
    float angleRadians = _angle * (PI / 180.0f); // Conversion en radians
    float arcLength = (WHEEL_DISTANCE_MM / 2.0f) * angleRadians; // Longueur de l'arc parcouru par chaque roue
    float revolutions = arcLength / circumferenceMM;
    return static_cast<long>(revolutions * STEPS_PER_REVOLUTION * stepMultiplier);
}
