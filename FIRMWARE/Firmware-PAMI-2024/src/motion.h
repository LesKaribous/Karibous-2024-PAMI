// motion.h
#ifndef MOTION_H
#define MOTION_H

#include <Arduino.h>
#include <AccelStepper.h>

#include "pins.h" 
#include "ihm.h" 

// Position absolue du robot
extern float x, y, rot;

// Déclaration des fonctions
void initMotion(); 
void enableMotors();
void disableMotors();
void setCoordinate(float _x, float _y, float _rot);
void go(float dist, float angle);
void goTo(float _x, float _y, float _rot);
void updateMotors();


#endif // MOTION_H