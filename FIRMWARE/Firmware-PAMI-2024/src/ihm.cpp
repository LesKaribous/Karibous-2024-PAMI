// ihm.cpp
#include "ihm.h"

// Initialisation des variables
bool team = TEAM_BLUE;
bool teamSelected = false;
bool modeDebug = false; // Mettre son robot en mode debug : oui / Mettre son robot en mode "des bugs" : Non - HistoriCode97 - 03/12/2023
bool modeDebugLCD = true;
byte robotNumber;

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R2); //HW stand for "Hardware"... You idiot
Adafruit_NeoPixel led = Adafruit_NeoPixel(1, ledStatus, NEO_GRB + NEO_KHZ800);

void initIHM(){
    // Init pins
    pinMode(ColorTeam,INPUT_PULLUP);
    pinMode(Tirette,INPUT_PULLUP);
    pinMode(BotNumb01,INPUT_PULLUP);
    pinMode(BotNumb02,INPUT_PULLUP);
    // Read robotNumber
    bool bit1 = !digitalRead(BotNumb01);
    bool bit2 = !digitalRead(BotNumb02);
    robotNumber = (bit2 << 1) | bit1;
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
  drawBackLcd();
  infoLCD("Hello !");
}

void drawBackLcd(){
  // Draw Bot Number
  u8g2.setFont(u8g2_font_ncenB08_tr);
  String stringValue = String(getRobotNumber());  // Convertir le byte en String
  const char* result = stringValue.c_str();       // Convertir la String en const char*
  u8g2.drawStr(0, 9, result);
  // Draw separators
  u8g2.drawLine(0,15,128,15);
  // Debug texte
  u8g2.setFont(u8g2_font_5x7_mf); // Mini font for debug - 6 heigh monospace
  u8g2.drawStr(0, 30, "debug: ");

  // Mettre à jour l'écran
  u8g2.sendBuffer();
}

void debugLCD(String message, u8g2_uint_t _y){
  u8g2.setFont(u8g2_font_5x7_mf); // Mini font for debug - 6 heigh monospace
  drawLCD(message, 35, _y);
}

void infoLCD(String message, u8g2_uint_t _y){
  u8g2.setFont(u8g2_font_5x7_mf); // Mini font for debug - 6 heigh monospace
  //u8g2.setFont(u8g2_font_ncenB08_tr);
  drawLCD(message, 55, _y);
}


void drawLCD(String message, u8g2_uint_t _x, u8g2_uint_t _y){
    const char* cstr = message.c_str();

    // Calculez la largeur et la hauteur de la chaîne à afficher
    //u8g2_uint_t width = u8g2.getStrWidth(cstr);
    u8g2_uint_t width = 128;
    u8g2_uint_t height = u8g2.getMaxCharHeight();

    // Position où le texte sera dessiné
    u8g2_uint_t x = _x;
    u8g2_uint_t y = _y;

    // Effacez seulement la zone où le texte sera dessiné
    u8g2.setDrawColor(0); // Couleur de fond pour "effacer"
    u8g2.drawBox(x, y - height, width, height);
    u8g2.setDrawColor(1); // Couleur de dessin pour le texte

    // Dessinez le texte
    u8g2.drawStr(x, y, cstr);

    // Mettre à jour l'écran
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

byte getRobotNumber(){
  return robotNumber;
}