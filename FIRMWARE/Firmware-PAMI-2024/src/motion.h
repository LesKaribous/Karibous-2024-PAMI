// motion.h
#ifndef MOTION_H
#define MOTION_H

#include <Arduino.h>
#include <AccelStepper.h>

#include "pins.h" 
#include "ihm.h" 

#define STEPS_PER_REVOLUTION 200    // Nombre de pas par tour du moteur
#define WHEEL_DIAMETER_MM 60.0f     // Diamètre de la roue en millimètres
#define WHEEL_DISTANCE_MM 85.0f     // Distance entre les roues en millimètres

#define MAX_SPEED       5000.0
#define ACCELERATION    2000.0

// Structure pour représenter une position et une orientation absolue du robot
struct Pose {
    float x; // Position en X (mm)
    float y; // Position en Y (mm)
    float rot; // Rotation en degrés

    // Méthodes pour définir les valeurs
    void setX(float _x) { x = _x; }
    void setY(float _y) { y = _y; }
    void setRot(float _rot) { rot = _rot; }

    // Méthodes pour obtenir les valeurs
    float getX() const { return x; }
    float getY() const { return y; }
    float getRot() const { return rot; }
}; 

// Structure pour représenter undéplacement polaire relatif du robot
struct PolarMove {
    float rotation1; // Première rotation pour s'aligner vers la cible
    float distance;  // Déplacement linéaire vers la cible
    float rotation2; // Rotation finale pour ajuster l'orientation
};

enum StepMode {
    EIGHTH_STEP,      // MS1 = GND, MS2 = GND
    HALF_STEP,        // MS1 = GND, MS2 = VIO
    QUARTER_STEP,     // MS1 = VIO, MS2 = GND
    SIXTEENTH_STEP    // MS1 = VIO, MS2 = VIO
};

// Déclaration des fonctions

void initMotion();
void setStepMode(StepMode mode); 
void enableMotors();
void disableMotors();
void setMaxSpeed(float _maxSpeed = MAX_SPEED);
void setAcceleration(float _acceleration = ACCELERATION);

void processMove();

long convertDistToStep(float _dist);
long convertAngleToStep(float angle);

// Déplacements relatifs
void go(float _dist);
void turn(float _angle);
void turnGo(float _angle, float _dist);

// Déplacements absolus
void goTo(Pose _target);
void goTo(float _x, float _y);
void goTo(float _x, float _y, float _rot);

// Converti la position demandée vers le targetPolarMove
void convertToPolar(Pose _target);
void convertToPolar(float _x, float _y);
void convertToPolar(float _x, float _y, float _rot);


#endif // MOTION_H