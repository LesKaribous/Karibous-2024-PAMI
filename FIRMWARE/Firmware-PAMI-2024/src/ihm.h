// ihm.h
#ifndef IHM_H
#define IHM_H

#include <Arduino.h>
#include <U8g2lib.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <esp_now.h>
#include <WiFi.h>

#include "pins.h" 

// Définitions des équipes
#define TEAM_BLUE 0
#define TEAM_YELLOW 1

// Définition des messages ESPNow et états
#define UNDEFINED 99
#define PAIRING 42
#define PAIRED 60
#define ARMED 70
#define READY 80
#define START_MATCH 126
#define MATCH_STARTED 150

// Déclaration des fonctions
void initIHM();
void initLCD();
void drawSplashScreen();
void pairingScreen();
void drawBackLcd();

bool initEspNow();
void printMacAdress();
bool BroadcastMessage(int orderMessage);
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);

void debug(String message);
void debugLCD(String message, u8g2_uint_t _y = 31);   // Affiche un message sur la ligne de debug (dessous)
void infoLCD(String message,  u8g2_uint_t _y = 13);   // Affiche un message sur la ligne info (dessus)
void drawLCD(String message, u8g2_uint_t _x, u8g2_uint_t _y); // Affiche un message et clear l'écran uniquement autour du message

bool checkColorTeam();
void initLedStatus();
bool getTirette();
byte readRobotNumber();
byte getRobotNumber();
bool getTeamColor();
int getRobotState();
void setRobotState(int state);

// Déclaration des variables globales (extern)
extern bool team;
//extern bool teamSelected;
//extern bool modeDebug; // Mettre son robot en mode debug : oui / Mettre son robot en mode "des bugs" : Non - HistoriCode97 - 03/12/2023
//extern bool modeDebugLCD;
//extern U8G2_SSD1306_128X32_UNIVISION_2_HW_I2C u8g2; // Objet pour l'écran LCD
//extern Adafruit_NeoPixel led; // Objet pour le LED

#endif // IHM_H
