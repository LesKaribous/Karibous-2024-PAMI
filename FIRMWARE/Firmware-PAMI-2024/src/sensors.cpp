// sensors.cpp
#include "sensors.h"

VL53L0X sensors[2];

void initSensor(){
    Wire.begin();
    // Disable/reset all sensors by driving their XSHUT pins low.
    pinMode(xshutPins[0],OUTPUT);
    pinMode(xshutPins[1],OUTPUT);
    digitalWrite(xshutPins[0],LOW);
    digitalWrite(xshutPins[1],LOW);

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