// ihm.h
#ifndef IHM_H
#define IHM_H

#include <Arduino.h>
#include <U8g2lib.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#include "pins.h" 

// Définitions des équipes
#define TEAM_BLUE 0
#define TEAM_YELLOW 1

// Déclaration des fonctions
void initIHM();
void initLCD();
void debug(String message);
void debugLCD(String message);
bool checkColorTeam();
void initLedStatus();
bool getTirette();

// Déclaration des variables globales (extern)
extern bool team;
//extern bool teamSelected;
//extern bool modeDebug; // Mettre son robot en mode debug : oui / Mettre son robot en mode "des bugs" : Non - HistoriCode97 - 03/12/2023
//extern bool modeDebugLCD;
//extern U8G2_SSD1306_128X32_UNIVISION_2_HW_I2C u8g2; // Objet pour l'écran LCD
//extern Adafruit_NeoPixel led; // Objet pour le LED

#endif // IHM_H
