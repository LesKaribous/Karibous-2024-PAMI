// actuators.h
#ifndef ACTUATORS_H
#define ACTUATORS_H

#include <Arduino.h>
#include <Servo.h> // use ServoESP32 lib - Version 1.1.1 - See https://github.com/RoboticsBrno/ServoESP32/issues/26 if compilation issues - On linux , use 1.1.0

#include "pins.h" 
#include "ihm.h" 

// Déclaration des fonctions
void initActuators();
void testAntenne();

#endif // ACTUATORS_H
