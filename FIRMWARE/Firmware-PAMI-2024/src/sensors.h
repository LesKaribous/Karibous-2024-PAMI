// sensors.h
#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>

#include "pins.h" 
#include "ihm.h" 

// Déclaration des fonctions
void initSensor();
void testSensor();

#endif // SENSORS_H
