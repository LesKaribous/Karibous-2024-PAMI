#include<U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
  #include<SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
  #include<Wire.h>
#endif

#include <AccelStepper.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <Servo.h> // use ServoEPS32 lib - Version 1.1.1 - See https://github.com/RoboticsBrno/ServoESP32/issues/26 if compilation issues

#define TEAM_BLUE 0
#define TEAM_YELLOW 1
#define MATCH_TIME 100000

#define PAMI_WAIT 0
#define PAMI_RUN 1
#define PAMI_END 2

#define TIME_START_PAMI 0
#define TIME_END_PAMI 10000

const int EN      = 48;
const int STEP_X  = 11;
const int DIR_X   = 10;
const int STEP_Y  = 14;
const int DIR_Y   = 13;

const int MS1 = 47;
const int MS2 = 21;

const int ColorTeam = 6;
const int Tirette   = 7;

const int pinServo  = 12;
const int frequency = 200; // Hz

const int LED_STATUS = 5;

const int xshutPins[2] = { 3,38 };

bool team = TEAM_BLUE;
bool teamSelected = false;
bool modeDebug = true; // Mettre son robot en mode debug : oui / Mettre son robot en mode "des bugs" : Non - HistoriCode97 - 03/12/2023
bool modeDebugLCD = true;

long elapsedTime  = 0;
long startTime    = 0;
int  statePAMI = PAMI_WAIT;

AccelStepper stepper1(AccelStepper::DRIVER, STEP_X, DIR_X);
AccelStepper stepper2(AccelStepper::DRIVER, STEP_Y, DIR_Y);

Adafruit_NeoPixel led = Adafruit_NeoPixel(1, LED_STATUS, NEO_GRB + NEO_KHZ800);

VL53L0X sensors[2];

Servo servoAntenne;

U8G2_SSD1306_128X32_UNIVISION_2_HW_I2C u8g2(U8G2_R0); //HW stand for "Hardware"... You idiot

void configureIO();
void initSensor();
void initLCD();
void testSensor();
void debug(String message);
void debugLCD(String message);
void checkColorTeam();
void initMotors();
void initLedStatus();
void waitStart();
void updateMatchTime();
void testMatch();
void testAntenne();

void setup() {
  configureIO();
  initSensor();
  initLCD();
  initMotors();
  initLedStatus();

  // Wait Start
  waitStart();

  //To test :
  stepper1.moveTo(1000);
  stepper2.moveTo(-1000);
}

void loop() 
{
  //updateMatchTime();
  //testAntenne();
  testSensor();
  //testMatch();
}

void configureIO(){
  Serial.begin(9600);
  Wire.begin();

  pinMode(ColorTeam,INPUT_PULLUP);
  pinMode(Tirette,INPUT_PULLUP);

  pinMode(EN,OUTPUT);
  pinMode(DIR_X,OUTPUT);
  pinMode(STEP_X,OUTPUT);
  pinMode(DIR_Y,OUTPUT);
  pinMode(STEP_Y,OUTPUT);

  pinMode(MS1,OUTPUT);
  pinMode(MS2,OUTPUT);

  // Disable/reset all sensors by driving their XSHUT pins low.
  pinMode(xshutPins[0],OUTPUT);
  pinMode(xshutPins[1],OUTPUT);
  digitalWrite(xshutPins[0],LOW);
  digitalWrite(xshutPins[1],LOW);

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

void initSensor(){

  for (uint8_t i = 0; i < 2; i++)
  {
    // Stop driving this sensor's XSHUT low. This should allow the carrier
    // board to pull it high. (We do NOT want to drive XSHUT high since it is
    // not level shifted.) Then wait a bit for the sensor to start up.
    pinMode(xshutPins[i], INPUT);
    delay(10);

    sensors[i].setTimeout(500);
    if (!sensors[i].init())
    {
      Serial.print("Failed to detect and initialize sensor ");
      Serial.println(i);
      //while (1);
    }
    else{
      Serial.print("Sensor ");
      Serial.print(i);
      Serial.println(" initialized");
    }

    // Each sensor must have its address changed to a unique value other than
    // the default of 0x29 (except for the last one, which could be left at
    // the default). To make it simple, we'll just count up from 0x2A.
    sensors[i].setAddress(0x2A + i);

    sensors[i].startContinuous(50);
    //sensors[i].setMeasurementTimingBudget(2000);
  }
}

void initLCD(){
  u8g2.begin();
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
  u8g2.drawStr(0,10,"Hello World!");	// write something to the internal memory
  u8g2.sendBuffer();					// transfer internal memory to the display
  delay(1000);  
}

void debugLCD(String message){
  const char* cstr = message.c_str();
  u8g2.clearBuffer();
  u8g2.drawStr(0,10,cstr);
  u8g2.sendBuffer();
  delay(1000);
}

void testSensor(){

  uint16_t sensor1 = sensors[0].readRangeContinuousMillimeters();
  uint16_t sensor2 = sensors[1].readRangeContinuousMillimeters();
  //uint16_t sensor1 = sensors[0].readRangeSingleMillimeters();
  //uint16_t sensor2 = sensors[1].readRangeSingleMillimeters();

  if (sensors[0].timeoutOccurred()) { debug(" TIMEOUT"); }
  if (sensors[1].timeoutOccurred()) { debug(" TIMEOUT"); }
  String str = String(sensor1) + "   " + String(sensor2);
  debug(str);
}

void debug(String message){
  if (modeDebug) Serial.println(message);
  if (modeDebugLCD) debugLCD(message);
}

void checkColorTeam(){
  bool temp = digitalRead(ColorTeam);
  if(team != temp || !teamSelected)
  {
    team = temp;
    teamSelected = true;
    led.setBrightness(50);
    if(team == TEAM_BLUE) led.setPixelColor(0,led.Color(0,0,255)); // LED en BLEU
    else led.setPixelColor(0,led.Color(255,255,0)); // LED en JAUNE
    led.show();
  }
}

void initMotors(){
  // Configure les pas
  digitalWrite(MS1,HIGH);
  digitalWrite(MS2,HIGH);
  // Desactive les moteurs
  digitalWrite(EN,HIGH);
  // Configure le moteur 1
  stepper1.setMaxSpeed(10000.0);
  stepper1.setAcceleration(5000.0);
  // Configure le moteur 2
  stepper2.setMaxSpeed(10000.0);
  stepper2.setAcceleration(5000.0);
}

void initLedStatus(){
  led.begin();
  led.setBrightness(50);
  led.setPixelColor(0,led.Color(255,255,255));
  led.show();
}

void waitStart(){
  //Attendre que la tirette soit insérée
  debug("Insert Tirette");
  while(digitalRead(Tirette)) {
    delay(250); 
    checkColorTeam();
  }
  //Attendre que la tirette soit retirée
  debug("Wait Start");
  while(!digitalRead(Tirette)) {
    delay(250); 
    checkColorTeam();
  }
  debug("Start Robot");
  startTime = millis();
}

void updateMatchTime(){
  elapsedTime = millis() - startTime ;
  if(elapsedTime>= TIME_START_PAMI)   statePAMI = true;
  if(elapsedTime>= TIME_END_PAMI)  statePAMI = false;
}

void testMatch(){
  if(statePAMI == PAMI_RUN){
    digitalWrite(EN,LOW);
    // Change direction at the limits
    if (stepper1.distanceToGo() == 0)
        stepper1.moveTo(-stepper1.currentPosition());
        // Change direction at the limits
    if (stepper2.distanceToGo() == 0)
        stepper2.moveTo(-stepper2.currentPosition());
    stepper1.run();
    stepper2.run();
  }
  else if (statePAMI == PAMI_END){
    digitalWrite(EN,HIGH); // Desactive les moteurs
    while(1); // Fin de match
  }
  else {
    digitalWrite(EN,HIGH); // Desactive les moteurs
  }  
}

void testAntenne(){
  servoAntenne.write(45);
  debug("Antenne 45");
  delay(1000);
  servoAntenne.write(120);
  debug("Antenne 120");
  delay(1000);
}
