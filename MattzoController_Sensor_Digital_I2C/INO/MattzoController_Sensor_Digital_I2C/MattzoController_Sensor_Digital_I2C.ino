// MattzoSensorController Firmware
// Author: Dr. Matthias Runte
// Copyright 2020 by Dr. Matthias Runte
// License:
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Edited by DjPredator69 14/02/2021 tested = OK
// Software prepered for 8 x PCF8574 = 64 Sensors
// NEED to check your adr of each PCF8574 and set the numbre of Sensors in config file
//
#define MATTZO_CONTROLLER_TYPE "MattzoSensorController"
#include <ESP8266WiFi.h>                          // WiFi library for ESP-8266
#include "MattzoSensorController_Configuration.h" // this file should be placed in the same folder
#include "MattzoController_Library.h"             // this file needs to be placed in the Arduino library folder

#include <Wire.h>                                 //
#include <PCF8574.h>                              //
PCF8574 PCF_01(0x38);                             // set adr from PCF8574   ==> 000
PCF8574 PCF_02(0x39);                             // set adr from PCF8574   ==> 100
PCF8574 PCF_03(0x40);                             // set adr from PCF8574   ==> 010
PCF8574 PCF_04(0x41);                             // set adr from PCF8574   ==> 110
PCF8574 PCF_05(0x42);                             // set adr from PCF8574   ==> 001
PCF8574 PCF_06(0x43);                             // set adr from PCF8574   ==> 101
PCF8574 PCF_07(0x44);                             // set adr from PCF8574   ==> 011
PCF8574 PCF_08(0x45);                             // set adr from PCF8574   ==> 111


// Time in milliseconds until release event is reported after sensor has lost contact
const int SENSOR_RELEASE_TICKS = 100;
bool sensorState[NUM_SENSORS];
int sensorTriggerState[NUM_SENSORS];
int lastSensorContactMillis[NUM_SENSORS];



void setup() {
  // initialize sensor pins
  /*
  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(SENSOR_PIN[i], INPUT_PULLUP);
    sensorState[i] = false;
    sensorTriggerState[i] = (SENSOR_PIN[i] == D8) ? HIGH : LOW;
  }
  */
  Wire.begin();
  
  delay(500);
  
  // load config from EEPROM, initialize Wifi, MQTT etc.
  setupMattzoController();

  delay(500);
  
  PCF_01.begin();
  if (!PCF_01.begin())
  {
    mcLog("1 could not initialize...");
  }else{
    mcLog("=>1 PCF8574 is initialized");  
  }
  if (!PCF_01.isConnected())
  {
    mcLog("=>(1) not connected");
    while(1);
  }else{
    mcLog("=>1 PCF8574 is connected");  
  }

  if(NUM_SENSORS > 8 && NUM_SENSORS < 16)
  {
    PCF_02.begin();
    if (!PCF_02.begin())
    {
      mcLog("(2) could not initialize...");
    }else{
      mcLog("=>2 PCF8574 are initialized");  
    }
    if (!PCF_02.isConnected())
    {
      mcLog("=>(2) not connected");
      while(1);
    }else{
      mcLog("=>2 PCF8574 are connected");  
    }
  }else{
    mcLog("=>only 1 PCF8574 is connected");
  }

  if(NUM_SENSORS > 16 && NUM_SENSORS < 24)
  {
    PCF_03.begin();
    if (!PCF_03.begin())
    {
      mcLog("(3) could not initialize...");
    }else{
      mcLog("=>3 PCF8574 are initialized");  
    }
    if (!PCF_03.isConnected())
    {
      mcLog("=>(3) not connected");
      while(1);
    }else{
      mcLog("=>3 PCF8574 are connected");  
    }
  }else{
    mcLog("=>only 2 PCF8574 is connected");
  }

  if(NUM_SENSORS > 24 && NUM_SENSORS < 32)
  {
    PCF_04.begin();
    if (!PCF_04.begin())
    {
      mcLog("(4) could not initialize...");
    }else{
      mcLog("=>4 PCF8574 are initialized");  
    }
    if (!PCF_04.isConnected())
    {
      mcLog("=>(4) not connected");
      while(1);
    }else{
      mcLog("=>4 PCF8574 are connected");  
    }
  }else{
    mcLog("=>only 3 PCF8574 is connected");
  }

  if(NUM_SENSORS > 32 && NUM_SENSORS < 40)
  {
    PCF_05.begin();
    if (!PCF_05.begin())
    {
      mcLog("(5) could not initialize...");
    }else{
      mcLog("=>5 PCF8574 are initialized");  
    }
    if (!PCF_05.isConnected())
    {
      mcLog("=>(5) not connected");
      while(1);
    }else{
      mcLog("=>5 PCF8574 are connected");  
    }
  }else{
    mcLog("=>only 4 PCF8574 is connected");
  }

  if(NUM_SENSORS > 40 && NUM_SENSORS < 48)
  {
    PCF_06.begin();
    if (!PCF_06.begin())
    {
      mcLog("(6) could not initialize...");
    }else{
      mcLog("=>6 PCF8574 are initialized");  
    }
    if (!PCF_06.isConnected())
    {
      mcLog("=>(6) not connected");
      while(1);
    }else{
      mcLog("=>6 PCF8574 are connected");  
    }
  }else{
    mcLog("=>only 5 PCF8574 is connected");
  }

  if(NUM_SENSORS > 48 && NUM_SENSORS < 56)
  {
    PCF_07.begin();
    if (!PCF_07.begin())
    {
      mcLog("(7) could not initialize...");
    }else{
      mcLog("=>7 PCF8574 are initialized");  
    }
    if (!PCF_07.isConnected())
    {
      mcLog("=>(7) not connected");
      while(1);
    }else{
      mcLog("=>7 PCF8574 are connected");  
    }
  }else{
    mcLog("=>only 6 PCF8574 is connected");
  }

  if(NUM_SENSORS > 56 )
  {
    PCF_08.begin();
    if (!PCF_08.begin())
    {
      mcLog("(8) could not initialize...");
    }else{
      mcLog("=>8 PCF8574 are initialized");  
    }
    if (!PCF_08.isConnected())
    {
      mcLog("=>(8) not connected");
      while(1);
    }else{
      mcLog("=>8 PCF8574 are connected");  
    }
  }else{
    mcLog("=>only 7 PCF8574 is connected");
  }


  
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // MattzoSensorControllers do not care about received mqtt messages.
  // => No operation required.
}

void sendSensorEvent2MQTT(int sensorPort, int sensorState) {
  String sensorRocId = MATTZO_CONTROLLER_TYPE + String(mattzoControllerId) + "-" + String(sensorPort + 1);  // e.g. "MattzoController12345-3"
  String stateString = sensorState ? "true" : "false";

  // compile mqtt message. Parameters:
  //   id: Combination of sensor name and port (e.g. MattzoController12345-3). The reported port (the "logic" port) is 1 count higher than the internal port number in the sensor, e.g. port 2 in the sensor equals 3 in Rocrail)
  //   bus: controller number
  //   address: port number (internal port number plus 1)
  // both id or bus/address can be used in Rocrail. If id is used, it superseeds the combination of bus and address
  String mqttMessage = "<fb id=\"" + sensorRocId + "\" bus=\"" + String(mattzoControllerId) + "\" addr=\"" + String(sensorPort + 1) + "\" state=\"" + stateString + "\"/>";
  mcLog("Sending MQTT message: " + mqttMessage);
  char mqttMessage_char[255];   // message is usually 61 chars, so 255 chars should be enough
  mqttMessage.toCharArray(mqttMessage_char, mqttMessage.length() + 1);
  mqttClient.publish("rocrail/service/client", mqttMessage_char);
}

// Switches LED on if one or more sensors has contact
// Switches LED off if no sensor has contact
void setLEDBySensorStates() {
  bool ledOnOff = false;
  for (int i = 0; i < NUM_SENSORS; i++) {
    if (sensorState[i]) {
      statusLEDState = true;
      return;
    }
  }
  statusLEDState = false;
}

void monitorSensors() {
  for (int i = 0; i < NUM_SENSORS; i++) {
    //int sensorValue = digitalRead(SENSOR_PIN[i]);
    //int sensorValue = PCF_01.read(IN[i]);
    
    int sensorValue;
    if(i <= 7)
    {
      i = i - 0;
      sensorValue = PCF_01.read(IN[i]);
    }else if(i >= 8 && i <= 15){
      i = i - 8;
      sensorValue = PCF_02.read(IN[i]);  
    }else if(i >= 16 && i <= 23){
      i = i - 16;
      sensorValue = PCF_03.read(IN[i]);  
    }else if(i >= 24 && i <= 31){
      i = i - 24;
      sensorValue = PCF_04.read(IN[i]);  
    }else if(i >= 32 && i <= 39){
      i = i - 32;
      sensorValue = PCF_05.read(IN[i]);  
    }else if(i >= 40 && i <= 47){
      i = i - 40;
      sensorValue = PCF_06.read(IN[i]);  
    }else if(i >= 48 && i <= 55){
      i = i - 48;
      sensorValue = PCF_07.read(IN[i]);  
    }else if(i >= 56){
      i = i - 56;
      sensorValue = PCF_08.read(IN[i]);  
    }

    if (sensorValue == sensorTriggerState[i]) {
      // Contact -> report contact immediately
      if (!sensorState[i]) {
        mcLog("Sensor " + String(i) + " triggered.");
        sendSensorEvent2MQTT(i, true);
        sensorState[i] = true;
      }
      lastSensorContactMillis[i] = millis();
    } else {
      // No contact for SENSOR_RELEASE_TICKS milliseconds -> report sensor has lost contact
      if (sensorState[i] && (millis() > lastSensorContactMillis[i] + SENSOR_RELEASE_TICKS)) {
        mcLog("Sensor " + String(i) + " released.");
        sendSensorEvent2MQTT(i, false);
        sensorState[i] = false;
      }
    }
  }

  setLEDBySensorStates();
}




void loop() {
  loopMattzoController();
  monitorSensors();
}
