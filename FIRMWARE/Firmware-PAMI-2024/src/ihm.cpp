// ihm.cpp
#include "ihm.h"

// Initialisation des variables
bool team = TEAM_BLUE;
bool teamSelected = false;
bool modeDebug = false; // Mettre son robot en mode debug : oui / Mettre son robot en mode "des bugs" : Non - HistoriCode97 - 03/12/2023
bool modeDebugLCD = true;
byte robotNumber;
int robotState = UNDEFINED;

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R2); //HW stand for "Hardware"... You idiot
Adafruit_NeoPixel led = Adafruit_NeoPixel(1, ledStatus, NEO_GRB + NEO_KHZ800);

// Définition variables pour ESPNow
uint8_t PAMI_1[] = {0xF4, 0x12, 0xFA, 0x53, 0x6E, 0x60}; // Sender
uint8_t PAMI_2[] = {0xF4, 0x12, 0xFA, 0x4C, 0x07, 0x94}; // Receiver
uint8_t PAMI_3[] = {0xF4, 0x12, 0xFA, 0x53, 0x6D, 0x08}; // Receiver

typedef struct message_struct {
  int order;
} message_struct;

message_struct message;

esp_now_peer_info_t peerInfo;

// --------------------------------------------------------------------

void initIHM(){
    // Init pins
    pinMode(ColorTeam,INPUT_PULLUP);
    pinMode(Tirette,INPUT_PULLUP);
    pinMode(BotNumb01,INPUT_PULLUP);
    pinMode(BotNumb02,INPUT_PULLUP);
    // Init functions
    Wire.begin();
    Serial.begin(115200);
    readRobotNumber();
    initLedStatus();
    initLCD();
    // Debug
    debug("PAMI Started");
}

void initLCD(){
  u8g2.begin();
  u8g2.clearBuffer();					// clear the internal memory
}

void pairingScreen(){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_streamline_interface_essential_wifi_t);
  u8g2.drawGlyphX2(0,42,0x0032); // Draw signal ESPNow
  // Mettre à jour l'écran
  u8g2.sendBuffer();
  delay(800);
  // Draw Text
  u8g2.setFont(u8g2_font_t0_22b_mf);
  u8g2.drawStr(49, 13, "ESPNow");
  u8g2.setFont(u8g2_font_5x7_mf);
  u8g2.drawStr(50, 21, "Pairing ...");
  u8g2.setFont(u8g2_font_tiny5_tf);
  u8g2.drawStr(50, 30, "Plug tirette to ignore");
  // Mettre à jour l'écran
  u8g2.sendBuffer();
  // Wait pairing or tirette
  while(!getTirette() && !initEspNow())
  {
    delay(250);
  }
  printMacAdress();
  u8g2.clearBuffer();
}

void drawSplashScreen(){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_streamline_coding_apps_websites_t);
  u8g2.drawGlyphX2(0,42,0x0043); // Draw LadyBug
  // Mettre à jour l'écran
  u8g2.sendBuffer();
  delay(800);
  // Draw Text
  u8g2.setFont(u8g2_font_t0_22b_mf);
  u8g2.drawStr(49, 13, "PAMI 24");
  u8g2.setFont(u8g2_font_5x7_mf);
  u8g2.drawStr(50, 21, "Les Karibous");
  // Créer une String avec la date et l'heure de compilation
  u8g2.setFont(u8g2_font_tiny5_tf);
  String compileDateTime = String(__DATE__) + " " + String(__TIME__);
  u8g2.drawStr(50, 30, compileDateTime.c_str());
  // Mettre à jour l'écran
  u8g2.sendBuffer();
  delay(2000);
  u8g2.clearBuffer();
}

void drawBackLcd(){
  //--------------------------------------------------------
  // Draw Bot Number
  String stringValue = String(getRobotNumber());  // Convertir le byte en String
  const char* result = stringValue.c_str();       // Convertir la String en const char*
  u8g2.setFont(u8g2_font_5x7_mf);
  u8g2.drawStr(0, 13, "bot");
  u8g2.setFont(u8g2_font_t0_22b_mf);
  u8g2.drawStr(14, 13, result);
  // Draw separators
  u8g2.drawLine(0,18,128,18);
  u8g2.drawLine(50,15,50,0);
  // Debug texte
  u8g2.setFont(u8g2_font_5x7_mf); // Mini font for debug - 6 heigh monospace
  u8g2.drawStr(0, 31, "debug: ");
  // Mettre à jour l'écran
  u8g2.sendBuffer();
}

bool initEspNow(){
  bool initState = false;
  bool addPeerState = true;
  bool messageState = true;
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) 
  {
    debug("Error init ESP-NOW !");
    initState = false;
  } else {initState = true;}
  // Si Init OK
  if(initState == true)
  {
    if(robotNumber == 1) //Si Robot Principal
    {
      esp_now_register_send_cb(OnDataSent);
      // register peer
      peerInfo.channel = 0;
      peerInfo.encrypt = false;
      // register first peer
      memcpy(peerInfo.peer_addr, PAMI_2, 6);
      if (esp_now_add_peer(&peerInfo) != ESP_OK)
      {
        Serial.println("Failed to add peer");
        addPeerState = false;
      }else{addPeerState = true;}
      // register second peer
      memcpy(peerInfo.peer_addr, PAMI_3, 6);
      if (esp_now_add_peer(&peerInfo) != ESP_OK)
      {
        Serial.println("Failed to add peer");
        addPeerState = false;
      }else{addPeerState = true;}
    } else {esp_now_register_recv_cb(OnDataRecv);}// get recv packer info
  }
  if(initState && addPeerState && robotNumber == 1)
  {
    messageState = BroadcastMessage(PAIRING);
    if (messageState) robotState = PAIRED;
  }
  return initState && addPeerState && messageState;
}

void printMacAdress(){
  Serial.println(WiFi.macAddress());
}

bool BroadcastMessage(int orderMessage){

  bool sendState = true;
  message.order = orderMessage;

  if(robotNumber == 1)
  {
    esp_err_t resultPami2 = esp_now_send(PAMI_2, (uint8_t *) &message, sizeof(message));
    esp_err_t resultPami3 = esp_now_send(PAMI_3, (uint8_t *) &message, sizeof(message));

    if (resultPami2 != ESP_OK || resultPami3 != ESP_OK ){
      if (resultPami2 != ESP_OK) Serial.println("Error sending the data to PAMI2");
      if (resultPami3 != ESP_OK) Serial.println("Error sending the data to PAMI3");
      sendState = false;
    }
    else
    {
      Serial.println("Sent with success");
      sendState = true;
    }
  }
  return sendState;
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&message, incomingData, sizeof(message));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("order: ");
  Serial.println(message.order);
  if (message.order == PAIRING) robotState = PAIRED;
  if (message.order == ARMED) robotState = READY;
  if (message.order == START_MATCH) robotState = MATCH_STARTED;
  Serial.println(robotState);
  Serial.println();
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
  return !digitalRead(Tirette);
}

byte readRobotNumber(){
  // Read robotNumber
  bool bit1 = !digitalRead(BotNumb01);
  bool bit2 = !digitalRead(BotNumb02);
  robotNumber = (bit2 << 1) | bit1;
  return robotNumber;
}

byte getRobotNumber(){
  return robotNumber;
}

bool getTeamColor(){
  return team;
}

int getRobotState(){
  return robotState;
}

void setRobotState(int state){
  robotState = state;
}