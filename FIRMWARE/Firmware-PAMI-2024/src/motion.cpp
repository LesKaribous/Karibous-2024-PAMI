// motion.h
#include "motion.h"

// Position absolue du robot
Pose currentPose  = {0.0f, 0.0f, 0.0f};
Pose targetPose   = {0.0f, 0.0f, 0.0f};
// Déplacement cible polaire
PolarMove targetMove = {0.0f, 0.0f, 0.0f};
float tempTargetRotation = 0.0f;
bool newPolarTarget = false;

StepMode currentStepMode = SIXTEENTH_STEP;
int stepMultiplier = 16;
float circumferenceMM = WHEEL_DIAMETER_MM * PI;

// create the stepper motor object
AccelStepper motor_G(AccelStepper::DRIVER, STEP_G, DIR_G);
AccelStepper motor_D(AccelStepper::DRIVER, STEP_D, DIR_D);

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
  setStepMode(QUARTER_STEP);
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
      digitalWrite(MS1, HIGH);
      digitalWrite(MS2, LOW);
      stepMultiplier = 2;
      break;
    case QUARTER_STEP:
      digitalWrite(MS1, LOW);
      digitalWrite(MS2, HIGH);
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

void go(float _dist){
  long stepValue = convertDistToStep(_dist);
  motor_G.move(stepValue);
  motor_D.move(-stepValue);
  processMove();
}

void turn(float _angle){
  long stepValue = convertAngleToStep(_angle);
  motor_G.move(stepValue);
  motor_D.move(stepValue);
  processMove();
}

void updateMotors(){
  motor_D.run();
  motor_G.run();
}

void setCurrentY(float _y){
  currentPose.setY(_y);
}

void setCurrentX(float _x){
  currentPose.setX(_x);
}

void setCurrentRot(float _rot){
  currentPose.setRot(_rot);
}

void processMove(){
  // WARNING : Blocking function
  // WIP
  debug("Processing Move...");
  while(motor_D.isRunning() || motor_G.isRunning()){
    updateMotors();
  }
  debug("Movement ok");
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

void convertToPolar(Pose _target){
  convertToPolar(_target.x, _target.y, _target.rot);
}

void convertToPolar(float _x, float _y){
  float dx = _x - currentPose.x;
  float dy = -(_y - currentPose.y);

  float targetAngleRadians = atan2(dy, dx);
  float currentRotRadians = currentPose.rot * (M_PI / 180.0f);

  targetMove.distance = sqrt(dx*dx + dy*dy);

  // Calculer la rotation la plus courte pour rotation1
  targetMove.rotation1 = (targetAngleRadians - currentRotRadians) * (180.0f / M_PI);
  if (targetMove.rotation1 > 180.0f) targetMove.rotation1 -= 360.0f;
  if (targetMove.rotation1 < -180.0f) targetMove.rotation1 += 360.0f;

  tempTargetRotation = targetAngleRadians * (180.0f / M_PI);

  targetMove.rotation2 = 0; // Pas de rotation finale

  newPolarTarget = true;
}

void convertToPolar(float _x, float _y, float _rot){
  float dx = _x - currentPose.x;
  float dy = -(_y - currentPose.y);

  float targetAngleRadians = atan2(dy, dx);
  float currentRotRadians = currentPose.rot * (M_PI / 180.0f);
  float targetRotRadians = _rot * (M_PI / 180.0f);

  targetMove.distance = sqrt(dx*dx + dy*dy);

  // Calculer la rotation la plus courte pour rotation1
  targetMove.rotation1 = (targetAngleRadians - currentRotRadians) * (180.0f / M_PI);
  if (targetMove.rotation1 > 180.0f) targetMove.rotation1 -= 360.0f;
  if (targetMove.rotation1 < -180.0f) targetMove.rotation1 += 360.0f;

  // Calculer la rotation la plus courte pour rotation2
  targetMove.rotation2 = (targetRotRadians - targetAngleRadians) * (180.0f / M_PI);
  if (targetMove.rotation2 > 180.0f) targetMove.rotation2 -= 360.0f;
  if (targetMove.rotation2 < -180.0f) targetMove.rotation2 += 360.0f;

  newPolarTarget = true;
}

void goTo(Pose _target){
  goTo(_target.x, _target.y, _target.rot);
}

void goTo(float _x, float _y){
  // TODO
  // WIP
  // Blocking --> to be changed
  convertToPolar(_x,_y);
  turn(targetMove.rotation1);
  go(targetMove.distance);
  currentPose.setX(_x);
  currentPose.setY(_y);
  currentPose.setRot(tempTargetRotation);
  newPolarTarget = false;
}

void goTo(float _x, float _y, float _rot){
  // WIP
  // Blocking --> to be changed
  convertToPolar(_x,_y,_rot);
  turn(targetMove.rotation1);
  go(targetMove.distance);
  currentPose.setX(_x);
  currentPose.setY(_y);
  turn(targetMove.rotation2);
  currentPose.setRot(_rot);
  newPolarTarget = false;
}