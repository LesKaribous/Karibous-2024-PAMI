// ihm.cpp
#include "ihm.h"

// Initialisation des variables
bool team = TEAM_BLUE;
bool teamSelected = false;
bool modeDebug = true; // Mettre son robot en mode debug : oui / Mettre son robot en mode "des bugs" : Non - HistoriCode97 - 03/12/2023
bool modeDebugLCD = true;

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R2); //HW stand for "Hardware"... You idiot
Adafruit_NeoPixel led = Adafruit_NeoPixel(1, ledStatus, NEO_GRB + NEO_KHZ800);

void initIHM(){
    // Init pins
    pinMode(ColorTeam,INPUT_PULLUP);
    pinMode(Tirette,INPUT_PULLUP);
    // Init functions
    Wire.begin();
    Serial.begin(115200);
    initLedStatus();
    initLCD();
    // Debug
    debug("PAMI Started");
}

void initLCD(){
  u8g2.begin();
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
  u8g2.drawStr(0,8,"Hello !");	// write something to the internal memory
  u8g2.sendBuffer();					// transfer internal memory to the display
}

void debugLCD(String message){
  const char* cstr = message.c_str();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);	// Mini font for debug
  u8g2.drawStr(0,30,cstr);
  u8g2.sendBuffer();
}

void debug(String message){
  if (modeDebug) Serial.println(message);
  if (modeDebugLCD) debugLCD(message);
}

bool checkColorTeam(){
  bool temp = digitalRead(ColorTeam);
  if(team != temp || !teamSelected)
  {
    team = temp;
    teamSelected = true;
    led.setBrightness(50);
    if(team == TEAM_BLUE) {
      led.setPixelColor(0,led.Color(0,0,255)); // LED en BLEU
      debug("Team Blue");
    }
    else {
      led.setPixelColor(0,led.Color(255,255,0)); // LED en JAUNE
      debug("Team Yellow");
    }
    led.show();
  }
  return team;
}

void initLedStatus(){
  led.begin();
  led.setBrightness(50);
  led.setPixelColor(0,led.Color(255,255,255));
  led.show();
}

bool getTirette(){
  return digitalRead(Tirette);
}