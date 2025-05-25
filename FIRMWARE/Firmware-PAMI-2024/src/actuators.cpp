// actuators.h
#include "actuators.h"

Servo servoAntenne;
int frequency = 200; // Hz

void initActuators(){
    servoAntenne.attach(
        pinServo,
        Servo::CHANNEL_NOT_ATTACHED, 
        Servo::DEFAULT_MIN_ANGLE, 
        Servo::DEFAULT_MAX_ANGLE, 
        Servo::DEFAULT_MIN_PULSE_WIDTH_US, 
        Servo::DEFAULT_MAX_PULSE_WIDTH_US,
        frequency
    );
}

void antennasUp(){
  servoAntenne.write(ANTENNA_UP);
  //debug("Antenna 120 (up)");
}

void antennasDown(){
  servoAntenne.write(ANTENNA_DOWN);
  //debug("Antenna 45 (down)");
}

void penDown(){
  antennasUp();
}

void penUp(){
  antennasDown();
}

void testAntennas(){
  antennasDown();
  delay(1000);
  antennasUp();
  delay(1000);
}