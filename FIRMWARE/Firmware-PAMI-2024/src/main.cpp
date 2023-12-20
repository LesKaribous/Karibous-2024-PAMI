#include <Arduino.h>
#include "pins.h"
#include "ihm.h"
#include "sensors.h"
#include "actuators.h"
#include "motion.h"

#define PAMI_WAIT 0
#define PAMI_RUN 1
#define PAMI_END 2

#define TIME_START_PAMI 0
#define TIME_END_PAMI 10000

#define MATCH_TIME 100000

TaskHandle_t Task1;

long elapsedTime  = 0;
long startTime    = 0;
int  statePAMI = PAMI_WAIT;

void waitStart();
void updateMatchTime();
void testMatch();

void Task1code(void* pvParameters);

void setup() {

  initIHM();
  initSensor();
  initMotion();
  initActuators();

  enableMotors();
  waitStart();

  xTaskCreatePinnedToCore(
    Task1code,   // Fonction de la tâche
    "Task1",     // Nom de la tâche
    10000,       // Taille de la pile de la tâche
    NULL,        // Paramètre d'entrée de la tâche
    1,           // Priorité de la tâche
    &Task1,      // Handle de la tâche
    0);          // Core où exécuter la tâche
}

void loop() 
{
  //updateMatchTime();
  //testAntenne();
  //testSensor();
  //testMatch();
  
  enableMotors();
  /*
  turn(3600);
  delay(1000);
  */

  
  goTo(100,0,0);
  goTo(0,0,0);
  /*
  goTo(100,100,90);
  goTo(100,100,180);
  goTo(100,0,0);
  */
  

  /*
  go(100);
  turn(90);
  turn(-90);
  go(-100);
  */
}

void waitStart(){
  //Attendre que la tirette soit insérée
  infoLCD("Insert Tirette");
  while(getTirette()) {
    delay(250); 
    checkColorTeam();
  }
  //Attendre que la tirette soit retirée
  infoLCD("Wait Start");
  while(!getTirette()) {
    delay(250); 
    checkColorTeam();
  }
  infoLCD("Start Robot");
  startTime = millis();
}

void updateMatchTime(){
  elapsedTime = millis() - startTime ;
  if(elapsedTime>= TIME_START_PAMI)   statePAMI = true;
  if(elapsedTime>= TIME_END_PAMI)     statePAMI = false;
}

void testMatch(){
  if(statePAMI == PAMI_RUN){
    enableMotors();
    // ToDo - Ecrire la strat
  }
  else if (statePAMI == PAMI_END){
    disableMotors(); // Desactive les moteurs
    while(1); // Fin de match
  }
  else {
    disableMotors(); // Desactive les moteurs
  }  
}

void Task1code(void* pvParameters) {
  for (;;) {
    // Code à exécuter en continu sur le core 0
    updateMotors();
    vTaskDelay(1 / portTICK_PERIOD_MS); // Laissez du temps à d'autres tâches - Suspends la tache pendant 1ms
    // TODO : 
    // - ne pas exécuter updateMotors tous le temps mais seulement quand on a besoisn d'un déplacement
    // - Modifier le vTaskDelay() lorsqu'on a pas besoins de la tache
    
  }
}