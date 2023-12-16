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

void testAntenne(){
  servoAntenne.write(45);
  debug("Antenne 45");
  delay(1000);
  servoAntenne.write(120);
  debug("Antenne 120");
  delay(1000);
}