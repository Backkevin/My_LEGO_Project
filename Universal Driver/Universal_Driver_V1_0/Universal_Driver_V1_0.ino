/*
* UNIVERSAL DRIVER 
* 
* Author: BACK KEVIN
* Copyright 20201 by Back Kevin
* License:
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
* 
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////START  SETTING
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MATTZO_CONTROLLER_TYPE "UniversalDriver"

#include <ESP8266WiFi.h>                          // WiFi library for ESP-8266
#include <Servo.h>                                // Servo library
#include <Wire.h>                                 // Built-in library for I2C

#include "SYSTEM.h"                               // this file should be placed in the same folder
#include "L9110.h"                                // this file should be placed in the same folder
#include "MCP23017.h"                             // this file should be placed in the same folder
#include "PCA9685.h"                              // this file should be placed in the same folder
#include "PCF8574.h"                              // this file should be placed in the same folder

#include "MattzoController_Library.h"             // this file needs to be placed in the Arduino library folder

#if USE_PCA9685
#include <Adafruit_PWMServoDriver.h>              // Adafruit PWM Servo Driver Library for PCA9685 port expander. Tested with version 2.4.0.
Adafruit_PWMServoDriver pca9685[NUM_PCA9685s];
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
#endif

#if USE_MCP23017
#include "Adafruit_MCP23017.h"
Adafruit_MCP23017 mcp23017[NUM_MCP23017s];
#endif

#if USE_L9110_BOARD
#include "Adafruit_MCP23017.h"
Adafruit_MCP23017 L9110B[NUM_L9110_BOARDs];
#endif


 
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












// ****************
// INPUT settings
// ****************

// Time in milliseconds until release event is reported after sensor has lost contact
const int SENSOR_RELEASE_TICKS = 100;
bool sensorState[NUM_SENSORS];
int sensorTriggerState[NUM_SENSORS];
int lastSensorContactMillis[NUM_SENSORS];


// ****************
// OUTPUT settings
// ****************



// ****************
// SWITCH settings
// ****************



// ****************
// CROSSING settings
// ****************



// ****************
// BRIDGE settings
// ****************



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////END SETTING
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////START SETUP
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {

Wire.begin();
  
delay(500);
  
// load config from EEPROM, initialize Wifi, MQTT etc.
setupMattzoController();

delay(500);


#if USE_PCA9685
  setupPCA9685();
#endif

#if USE_MCP23017
  setupMCP23017();
#endif

#if USE_L9110_BOARD
  setupMCP23017();
#endif

#if USE_PCF8574
  setupPCF8574();                           
#endif
  
  
  switch (USED_SYS) {
  case 1: // INPUTS
    mcLog(" Configured as INPUT"); 
    #if USE_PCF8574
      mcLog(" USE PCF8574"); 
    #endif
    
    #if USE_MCP23017
      mcLog(" USE MCP23017");
      //pullups aktivieren
    #endif   
    
    break;
    
  case 2: // OUTPUTS
    mcLog(" Configured as OUTPUT"); 
    #if USE_PCF8574
      mcLog(" USE PCF8574"); 
    #endif
    
    #if USE_MCP23017
      mcLog(" USE MCP23017");
      //pullups aktivieren
    #endif
    break;
    
  case 3: // SWITCHES
    mcLog(" Configured as SWITCH"); 

    #if USE_PCF8574
      mcLog(" USE PCF8574"); 
    #endif
    
    #if USE_MCP23017
      mcLog(" USE MCP23017");
      //pullups aktivieren
    #endif 
    
    break;
    
  case 4: // CROSSINGS
    mcLog(" Configured as CROSSING"); 

    #if USE_PCF8574
      mcLog(" USE PCF8574"); 
    #endif
    
    #if USE_MCP23017
      mcLog(" USE MCP23017");
      //pullups aktivieren
    #endif 
    
    break;
    
  case 5: // BRIDGES
    mcLog(" Configured as BRIDGE"); 

    #if USE_PCF8574
      mcLog(" USE PCF8574"); 
    #endif
    
    #if USE_MCP23017
      mcLog(" USE MCP23017");
      //pullups aktivieren
    #endif 
    
    break;
    
  default: // DEFAULT
    // Statement(s)
    break; 
  }

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////END SETUP
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#if USE_PCA9685
void setupPCA9685() {
  // Initialize PWM Servo Driver object (for PCA9685)
  for (int p = 0; p < NUM_PCA9685s; p++) {
    pca9685[p] = Adafruit_PWMServoDriver(PCA9685_ADR[p]);
    pca9685[p].begin();
    pca9685[p].setOscillatorFrequency(27000000);
    pca9685[p].setPWMFreq(SERVO_FREQ);
    
    if (PCA9685_OE_PIN_INSTALLED) {
      pinMode(PCA9685_OE_PIN[p], OUTPUT);
      setServoSleepMode(true);
    }
    
    delay(10);
  }
}
#endif


#if USE_MCP23017
void setupMCP23017() {
  for (int m = 0; m < NUM_MCP23017s; m++) {
    mcp23017[m] = Adafruit_MCP23017(MCP23017_ADR[m]);
    mcp23017[m].begin();
  }
}
#endif


#if USE_L9110_BOARD
void setupL9110() {
  for (int r = 0; r < NUM_L9110_BOARDs; r++) {
    L9110B[r] = Adafruit_MCP23017(L9110_BOARD_ADR[r]);
    L9110B[r].begin();
  }
}
#endif


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
















////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////START  INPUTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if ONLY_INPUTS

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
    int b;
    int sensorValue;
    
    if(i <= 7)
    {
      b = i;
      sensorValue = PCF_01.read(IN[b]);
    }else if(i >= 8 && i <= 15){
      b = i - 8;
      sensorValue = PCF_02.read(IN[b]);  
    }else if(i >= 16 && i <= 23){
      b = i - 16;
      sensorValue = PCF_03.read(IN[b]);  
    }else if(i >= 24 && i <= 31){
      b = i - 24;
      sensorValue = PCF_04.read(IN[b]);  
    }else if(i >= 32 && i <= 39){
      b = i - 32;
      sensorValue = PCF_05.read(IN[b]);  
    }else if(i >= 40 && i <= 47){
      b = i - 40;
      sensorValue = PCF_06.read(IN[b]);  
    }else if(i >= 48 && i <= 55){
      b = i - 48;
      sensorValue = PCF_07.read(IN[b]);  
    }else if(i >= 56){
      b = i - 56;
      sensorValue = PCF_08.read(IN[b]);  
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

#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////END  INPUTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////START  SIGNALS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if ONLY_SIGNALS

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  char msg[length+1];
  for (int i = 0; i < length; i++) {
      msg[i] = (char)payload[i];
  }
  msg[length] = '\0';

  mcLog("Received MQTT message [" + String(topic) + "]: " + String(msg));

  XMLDocument xmlDocument;
  if(xmlDocument.Parse(msg)!= XML_SUCCESS) {
    mcLog("Error parsing XML.");
    return;
  }
  mcLog("Parsing XML successful.");

  XMLElement * element = xmlDocument.FirstChildElement("co");
  if (element == NULL) {
    mcLog("<co> node not found. Message disregarded.");
    return;
  }

  mcLog("<co> node found.");

  // query addr attribute. This is the MattzoController id.
  // If this does not equal the ControllerNo of this controller, the message is disregarded.
  int rr_addr = 0;
  if (element->QueryIntAttribute("addr", &rr_addr) != XML_SUCCESS) {
    mcLog("addr attribute not found or wrong type. Message disregarded.");
    return;
  }
  mcLog("addr: " + String(rr_addr));
  if (rr_addr != mattzoControllerId) {
    mcLog("Message disgarded, as it is not for me (" + String(mattzoControllerId) + ")");
    return;
  }

  // query port attribute. This is port id of the port to which the switch is connected.
  // If the controller does not have such a port, the message is disregarded.
  int rr_port = 0;
  if (element->QueryIntAttribute("port", &rr_port) != XML_SUCCESS) {
    mcLog("port attribute not found or wrong type. Message disregarded.");
    return;
  }
  mcLog("port: " + String(rr_port));
  if (rr_port < 1 || rr_port > NUM_SIGNALPORTS) {
    mcLog("Message disgarded, as this controller does not have such a port.");
    return;
  }

  // query cmd attribute. This is the desired switch setting and can either be "turnout" or "straight".
  const char * rr_cmd = "-unknown-";
  if (element->QueryStringAttribute("cmd", &rr_cmd) != XML_SUCCESS) {
    mcLog("cmd attribute not found or wrong type.");
    return;
  }
  mcLog("cmd: " + String(rr_cmd));

  // set signal LED for the port on/off
  if (strcmp(rr_cmd, "on")==0) {
    setSignalLED(rr_port - 1, true);
  } else if (strcmp(rr_cmd, "off")==0) {
    setSignalLED(rr_port - 1, false);
  } else {
    mcLog("Signal port command unknown - message disregarded.");
  }
}

void setSignalLED(int index, bool ledState) {
  //digitalWrite(SIGNALPORT_PIN[index], ledState ? LOW : HIGH);
  //PCF_01.write(OUT[index], ledState ? LOW : HIGH);

  if(index <= 7)
    {
      index = index - 0;
      PCF_01.write(OUT[index], ledState ? LOW : HIGH);
    }else if(index >= 8 && index <= 15){
      index = index - 8;
      PCF_02.write(OUT[index], ledState ? LOW : HIGH); 
    }else if(index >= 16 && index <= 23){
      index = index - 16;
      PCF_03.write(OUT[index], ledState ? LOW : HIGH);
    }else if(index >= 24 && index <= 31){
      index = index - 24;
      PCF_04.write(OUT[index], ledState ? LOW : HIGH);
    }else if(index >= 32 && index <= 39){
      index = index - 32;
      PCF_05.write(OUT[index], ledState ? LOW : HIGH);
    }else if(index >= 40 && index <= 47){
      index = index - 40;
      PCF_06.write(OUT[index], ledState ? LOW : HIGH);
    }else if(index >= 48 && index <= 55){
      index = index - 48;
      PCF_07.write(OUT[index], ledState ? LOW : HIGH);
    }else if(index >= 56){
      index = index - 56;
      PCF_08.write(OUT[index], ledState ? LOW : HIGH);
    }

    
}

#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////END  SIGNALS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////START  SWITCHES
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if ONLY_SWITCHES



#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////END  SWITCHES
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////START  CROSSINGS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if ONLY_CROSSINGS



#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////END  CROSSINGS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////START  BRIDGES
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if ONLY_BRIDGES



#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////END  BRIDGES
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////














////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////START LOOP
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {

  loopMattzoController();
   
  switch (USED_SYS) {
  case 1: // INPUTS
    monitorSensors();
    break;
    
  case 2: // OUTPUTS
    // Statement(s)
    break;
    
  case 3: // SWITCHES
    // Statement(s)
    break;
    
  case 4: // CROSSINGS
    // Statement(s)
    break;
    
  case 5: // BRIDGES
    // Statement(s)
    break;
    
  default: // DEFAULT
    // Statement(s)
    break; 
  }

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////END LOOP
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
