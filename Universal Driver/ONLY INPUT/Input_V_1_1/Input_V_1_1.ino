/*
 * written for PCF8574 and MCP23017 ,but not tested
 */

#define MATTZO_CONTROLLER_TYPE "OIN"


#include <ESP8266WiFi.h>                          // WiFi library for ESP-8266
#include <Wire.h>                                 // Built-in library for I2C
#include "Config.h"                               // this file should be placed in the same folder
#include "MattzoController_Library.h"             // this file needs to be placed in the Arduino library folder

// ******************
// PCF8574 settings
// ******************

#if USE_PCF8574
#include <PCF8574.h> 
PCF8574 PCF_01(PCF8574_ADR[0]);                             // set adr from PCF8574   ==> 000 
PCF8574 PCF_02(PCF8574_ADR[1]);                             // set adr from PCF8574   ==> 100 
PCF8574 PCF_03(PCF8574_ADR[2]);                             // set adr from PCF8574   ==> 010 
PCF8574 PCF_04(PCF8574_ADR[3]);                             // set adr from PCF8574   ==> 110 
PCF8574 PCF_05(PCF8574_ADR[4]);                             // set adr from PCF8574   ==> 001 
PCF8574 PCF_06(PCF8574_ADR[5]);                             // set adr from PCF8574   ==> 101 
PCF8574 PCF_07(PCF8574_ADR[6]);                             // set adr from PCF8574   ==> 011 
PCF8574 PCF_08(PCF8574_ADR[7]);                             // set adr from PCF8574   ==> 111               
#endif

// ******************
// MCP23017 settings
// ******************

#if USE_MCP23017
#include "Adafruit_MCP23X17.h"
Adafruit_MCP23X17 mcp23017[NUM_MCP23017s];
#endif


// ******************
// INPUT settings
// ******************

// Time in milliseconds until release event is reported after sensor has lost contact
const int SENSOR_RELEASE_TICKS = 100;
bool sensorState[NUM_SENSORS];
int sensorTriggerState[NUM_SENSORS];
int lastSensorContactMillis[NUM_SENSORS];


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////START SETUP
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  
Wire.begin();
  
delay(500);
  
// load config from EEPROM, initialize Wifi, MQTT etc.
setupMattzoController();

delay(500);

mcLog(" Configured as INPUT"); 

#if USE_PCA9685
  mcLog(" USE PCF8574"); 
  setupPCA9685();
#endif

#if USE_MCP23017
  mcLog(" USE MCP23017");
  setupMCP23017();
#endif

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////END SETUP
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#if USE_PCF8574
void setupPCF8574() {
  if(NUM_PCF8574s >= 1){PCF_01.begin();}                        
  if(NUM_PCF8574s >= 2){PCF_02.begin();}                             
  if(NUM_PCF8574s >= 3){PCF_03.begin();}                              
  if(NUM_PCF8574s >= 4){PCF_04.begin();}                             
  if(NUM_PCF8574s >= 5){PCF_05.begin();}                             
  if(NUM_PCF8574s >= 6){PCF_06.begin();}                             
  if(NUM_PCF8574s >= 7){PCF_07.begin();}                            
  if(NUM_PCF8574s >= 8){PCF_08.begin();}      
}
#endif


#if USE_MCP23017
void setupMCP23017() {

  int count = 0;
    
  for (int m = 0; m < NUM_MCP23017s; m++) {
    mcp23017[m] = Adafruit_MCP23X17();
    mcp23017[m].begin_I2C();
    
    for (int i = 0; i < 15; i++) {
      mcp23017[m].pinMode(IN_MCP[i], INPUT_PULLUP);
      sensorTriggerState[count] = LOW;
      count++;
      sensorState[count] = false;
    }
    
  }
  
}
#endif


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

int i = 0;
int sensorValue;

#if USE_PCF8574
  for (int x = 0; x < NUM_PCF8574s; x++) {
    
    for (int a = 0; a < 16; a++) {
           if(x == 0){sensorValue = PCF_01.read(IN_PCF[a]);}                        
      else if(x == 1){sensorValue = PCF_02.read(IN_PCF[a]);}                             
      else if(x == 2){sensorValue = PCF_03.read(IN_PCF[a]);}                              
      else if(x == 3){sensorValue = PCF_04.read(IN_PCF[a]);}                             
      else if(x == 4){sensorValue = PCF_05.read(IN_PCF[a]);}                             
      else if(x == 5){sensorValue = PCF_06.read(IN_PCF[a]);}                             
      else if(x == 6){sensorValue = PCF_07.read(IN_PCF[a]);}                            
      else if(x == 7){sensorValue = PCF_08.read(IN_PCF[a]);} 
      i++;
    }
    
  }
#endif

#if USE_MCP23017
  for (int x = 0; x < NUM_MCP23017s; x++) {
        
    for (int a = 0; a < 16; a++) {
      sensorValue = mcp23017[x].digitalRead(IN_MCP[a]);
      i++;
    }
    
  }
#endif


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
  

  setLEDBySensorStates();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////START LOOP
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  
  loopMattzoController();
  monitorSensors();

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////END LOOP
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
