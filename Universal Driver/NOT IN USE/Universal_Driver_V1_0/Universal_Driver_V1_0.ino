/*
* UNIVERSAL DRIVER 
* 
* Author: BACK KEVIN
* Copyright 2021 by Back Kevin
* License:
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
* 
*/



/*

intput ok to test with PCF8574
output ok to test with PCF8574
switch ok to test with PCA9685

crossing work on it

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

// Default values for TrixBrix switches (in case servo angles are not transmitted)
const int SERVO_MIN_ALLOWED = 30;   // minimum accepted servo angle from Rocrail. Anything below this value is treated as misconfiguration and is neglected and reset to SERVO_MIN.
const int SERVO_MIN = 79;           // a good first guess for the minimum angle of TrixBrix servos is 79
const int SERVO_START = 83;         // position after boot-up. For TrixBrix servos, this is more or less the middle position
const int SERVO_MAX = 87;           // a good first guess for the maximum angle of TrixBrix servos is 87
const int SERVO_MAX_ALLOWED = 100;  // maximum accepted servo angle from Rocrail. Anything above this value is treated as misconfiguration and is neglected and reset to SERVO_MAX.

// Delay between two switch operations
const int SWITCH_DELAY = 200;

// Time after servo operation until servo power is switched off (in milliseconds; 3000 = 3 sec.)
// Presently only supported when using PCA9685
const int SERVOSLEEPMODEAFTER_MS = 500;

// time when servos will go to sleep mode
bool servoSleepMode = false;
unsigned long servoSleepModeFrom_ms = 0;

// ****************
// CROSSING settings
// ****************

const int SERVO_MIN_2 = 90;           // a good first guess for the minimum angle of TrixBrix servos is 70
const int SERVO_MAX_2 = 145;           // a good first guess for the maximum angle of TrixBrix servos is 90

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 500;           // interval at which to blink (milliseconds)


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
  


#if ONLY_INPUTS
    mcLog(" Configured as INPUT"); 
    #if USE_PCF8574
      mcLog(" USE PCF8574"); 
    #endif
    
    #if USE_MCP23017
      mcLog(" USE MCP23017");
      //pullups aktivieren
    #endif
#endif

#if ONLY_SIGNALS
    mcLog(" Configured as OUTPUT"); 
    #if USE_PCF8574
      mcLog(" USE PCF8574"); 
    #endif
    
    #if USE_MCP23017
      mcLog(" USE MCP23017");
      //pullups aktivieren
    #endif
#endif

#if ONLY_SWITCHES
    mcLog(" Configured as SWITCH"); 

    #if USE_PCF8574
      mcLog(" USE PCF8574"); 
    #endif
    
    #if USE_MCP23017
      mcLog(" USE MCP23017");
      //pullups aktivieren
    #endif 
#endif

#if ONLY_CROSSINGS
    mcLog(" Configured as CROSSING"); 

    #if USE_PCF8574
      mcLog(" USE PCF8574"); 
    #endif
    
    #if USE_MCP23017
      mcLog(" USE MCP23017");
      //pullups aktivieren
    #endif 
#endif
  
#if ONLY_BRIDGES
    mcLog(" Configured as BRIDGE"); 

    #if USE_PCF8574
      mcLog(" USE PCF8574"); 
    #endif
    
    #if USE_MCP23017
      mcLog(" USE MCP23017");
      //pullups aktivieren
    #endif 
#endif

  
#if ONLY_ELEVATOR
    mcLog(" Configured as ELEVATOR"); 

    #if USE_PCF8574
      mcLog(" USE PCF8574"); 
    #endif
    
    #if USE_MCP23017
      mcLog(" USE MCP23017");
      //pullups aktivieren
    #endif 
#endif


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
      setServoSleepMode(true, p);
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

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  char msg[length + 1];
  for (int i = 0; i < length; i++) {
    msg[i] = (char)payload[i];
  }
  msg[length] = '\0';

  mcLog("Received MQTT message [" + String(topic) + "]: " + String(msg));

  XMLDocument xmlDocument;
  if(xmlDocument.Parse(msg)!= XML_SUCCESS){
    mcLog("Error parsing.");
    return;
  }
  mcLog("Parsing XML successful.");

  XMLElement *element;
  element = xmlDocument.FirstChildElement("sw");
  if (element != NULL) {
    mcLog("<sw> node found.");

    // query addr1 attribute. This is the MattzoController id.
    // If this does not equal the mattzoControllerId of this controller, the message is disregarded.
    int rr_addr1 = 0;
    if (element->QueryIntAttribute("addr1", &rr_addr1) != XML_SUCCESS) {
      mcLog("addr1 attribute not found or wrong type. Message disregarded.");
      return;
    }
    mcLog("addr1: " + String(rr_addr1));
    if (rr_addr1 != mattzoControllerId) {
      mcLog("Message disgarded, as it is not for me (" + String(mattzoControllerId) + ")");
      return;
    }

    // query port1 attribute. This is port id of the port to which the switch is connected.
    // If the controller does not have such a port, the message is disregarded.
    int rr_port1 = 0;
    if (element->QueryIntAttribute("port1", &rr_port1) != XML_SUCCESS) {
      mcLog("port1 attribute not found or wrong type. Message disregarded.");
      return;
    }
    mcLog("port1: " + String(rr_port1));
    if ((rr_port1 < 1 || rr_port1 > 32) && (rr_port1 < 1001 || rr_port1 > 1018)) {
      mcLog("Message disgarded, this controller does not have such a port.");
      return;
    }

    // query cmd attribute. This is the desired switch setting and can either be "turnout" or "straight".
    const char* rr_cmd = "-unknown-";
    if (element->QueryStringAttribute("cmd", &rr_cmd) != XML_SUCCESS) {
      mcLog("cmd attribute not found or wrong type.");
      return;
    }
    mcLog("cmd: " + String(rr_cmd));

    // query param1 attribute. This is the "straight" position of the switch servo motor.
    // defaults to SERVO_MIN
    int rr_param1 = SERVO_MIN;
    if (element->QueryIntAttribute("param1", &rr_param1) != XML_SUCCESS) {
      mcLog("param1 attribute not found or wrong type. Using default value.");
    }
    if (rr_param1 < SERVO_MIN_ALLOWED || rr_param1 > SERVO_MAX_ALLOWED) {
      // Reset angle back to standard if angle is too small
      // User has obviously forgotten to configure servo angle in Rocrail properly
      // To protect the servo, the default value is used
      mcLog("param1 attribute out of bounds. Using default value.");
      rr_param1 = SERVO_MIN;
    }
    mcLog("param1: " + String(rr_param1));

    // query value1 attribute. This is the "turnout" position of the switch servo motor.
    // defaults to SERVO_MAX
    int rr_value1 = SERVO_MAX;
    if (element->QueryIntAttribute("value1", &rr_value1) != XML_SUCCESS) {
      mcLog("value1 attribute not found or wrong type. Using default value.");
    }
    if (rr_value1 < SERVO_MIN_ALLOWED || rr_value1 > SERVO_MAX_ALLOWED) {
      // Reset angle back to standard if angle is too small
      // User has obviously forgotten to configure servo angle in Rocrail properly
      // To protect the servo, the default value is used
      mcLog("value1 attribute out of bounds. Using default value.");
      rr_value1 = SERVO_MAX;
    }
    mcLog("value1: " + String(rr_value1));

    // check command string and prepare servo angle
    // servo angle will only be used to flip a standard or one side of a triple switch - not for double slip switches!
    int switchCommand;
    int servoAngle;
    if (strcmp(rr_cmd, "straight") == 0) {
      switchCommand = 1;
      servoAngle = rr_param1;
    }
    else if (strcmp(rr_cmd, "turnout") == 0) {
      switchCommand = 0;
      servoAngle = rr_value1;
    }
    else {
      mcLog("Switch command unknown - message disregarded.");
      return;
    }



    for (int i = 0; i < NUM_PCA9685s; i++) {
      
      if (rr_port1 >= (i*10)+1001 && rr_port1 <= (i*10)+1008) {
        int servoPort1;
        int servoPort2;

        // port 1001 -> double slip switch 1 / side A / ports 1 and 2
        if (rr_port1 == (i*10)+1001) {
          servoPort1 = 1;
          servoPort2 = 2;  
        }
        // port 1002 -> double slip switch 1 / side B / ports 3 and 4
        else if (rr_port1 == (i*10)+1002) {
          servoPort1 = 3;
          servoPort2 = 4;
        }
        // port 1003 -> double slip switch 2 / side A / ports 5 and 6
        else if (rr_port1 == (i*10)+1003) {
          servoPort1 = 5;
          servoPort2 = 6;
        }
        // port 1004 -> double slip switch 2 / side B / ports 7 and 8
        else if (rr_port1 == (i*10)+1004) {
          servoPort1 = 7;
          servoPort2 = 8;
        }
        // port 1005 -> double slip switch 3 / side A / ports 1 and 2
        else if (rr_port1 == (i*10)+1005) {
          servoPort1 = 9;
          servoPort2 = 10;
        }
        // port 1006 -> double slip switch 3 / side B / ports 3 and 4
        else if (rr_port1 == (i*10)+1006) {
          servoPort1 = 11;
          servoPort2 = 12;
        }
        // port 1007 -> double slip switch 4 / side A / ports 5 and 6
        else if (rr_port1 == (i*10)+1007) {
          servoPort1 = 13;
          servoPort2 = 14;
        }
        // port 1008 -> double slip switch 4 / side B / ports 7 and 8
        else if (rr_port1 == (i*10)+1008) {
          servoPort1 = 15;
          servoPort2 = 16;
        }

        mcLog("Turning double slip switch servos on port " + String(servoPort1) + " and " + String(servoPort2) + " to angle " + String(servoAngle));
        setServoAngle(servoPort1 - 1, servoAngle, i);
        setServoAngle(servoPort2 - 1, servoAngle, i);
      }
      else if(rr_port1 >= 1 && rr_port1 <= 16) {
        mcLog("Turning servo on port " + String(rr_port1) + " to angle " + String(servoAngle));
        setServoAngle(rr_port1 -1, servoAngle, i);
      }
      else if(rr_port1 >= 17 && rr_port1 <= 128) {
        mcLog("Turning servo on port " + String(rr_port1-(16*i)) + " to angle " + String(servoAngle));
        setServoAngle((rr_port1-(16*i)) -1, servoAngle, i);
      }
      
    }

    
    return;
    // end of switch command handling
  }

  element = xmlDocument.FirstChildElement("co");
  if (element != NULL) {
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
    

    // query cmd attribute. This is the desired switch setting and can either be "turnout" or "straight".
    const char* rr_cmd = "-unknown-";
    if (element->QueryStringAttribute("cmd", &rr_cmd) != XML_SUCCESS) {
      mcLog("cmd attribute not found or wrong type.");
      return;
    }
    mcLog("cmd: " + String(rr_cmd));

    
    return;
    // end of signal handling
  }

  mcLog("No <sw> or <co> node found. Message disregarded.");
}










// sets the servo arm to a desired angle
void setServoAngle(int servoIndex, int servoAngle, int PCAnr) {
  if (servoIndex >= 0 && servoIndex < NUM_SWITCHPORTS) {
    setServoSleepMode(false, PCAnr);
    pca9685[PCAnr].setPWM(SWITCHPORT_PIN[servoIndex], 0, mapAngle2PulseLength(servoAngle));   
    delay(SWITCH_DELAY);
  }
  else {
    // this should not happen
    mcLog("WARNING: servo index " + String(servoIndex) + " out of range!");
  }
}










// converts a desired servo angle (0� - 180�) into a pwm pulse length (required for PCA9685)
int mapAngle2PulseLength(int angle) {
  const int PULSE_MIN = 0;
  const int PULSE_MAX = 600;
  return map(angle, 0, 180, PULSE_MIN, PULSE_MAX);
}








// Switches servo power supply on or off (presently supported for PCA9685 only)
void setServoSleepMode(bool onOff, int PCAnr) {
  if (PCA9685_OE_PIN_INSTALLED) {
    if (servoSleepMode != onOff) {
      digitalWrite(PCA9685_OE_PIN[PCAnr], onOff ? HIGH : LOW);                                                             
      servoSleepMode = onOff;
      if (onOff) {
        mcLog("Servo power turned off.");
      } else {
        mcLog("Servo power turned on.");
      }
    }
    if (!onOff) {
      servoSleepModeFrom_ms = millis() + SERVOSLEEPMODEAFTER_MS;
    }
  }
}

// Checks if servos can be set to sleep mode (presently supported for PCA9685 only)
void checkEnableServoSleepMode() {
  for (int i = 0; i < NUM_PCA9685s; i++) {
    if (millis() > servoSleepModeFrom_ms) {
      setServoSleepMode(true, i);
    }
  }
}

#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////END  SWITCHES
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////START  CROSSINGS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if ONLY_CROSSINGS

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  char msg[length + 1];
  for (int i = 0; i < length; i++) {
    msg[i] = (char)payload[i];
  }
  msg[length] = '\0';

  mcLog("Received MQTT message [" + String(topic) + "]: " + String(msg));

  XMLDocument xmlDocument;
  if(xmlDocument.Parse(msg)!= XML_SUCCESS){
    mcLog("Error parsing.");
    return;
  }
  mcLog("Parsing XML successful.");

  XMLElement *element;
  element = xmlDocument.FirstChildElement("sw");
  if (element != NULL) {
    mcLog("<sw> node found.");

    // query addr1 attribute. This is the MattzoController id.
    // If this does not equal the mattzoControllerId of this controller, the message is disregarded.
    int rr_addr1 = 0;
    if (element->QueryIntAttribute("addr1", &rr_addr1) != XML_SUCCESS) {
      mcLog("addr1 attribute not found or wrong type. Message disregarded.");
      return;
    }
    mcLog("addr1: " + String(rr_addr1));
    if (rr_addr1 != mattzoControllerId) {
      mcLog("Message disgarded, as it is not for me (" + String(mattzoControllerId) + ")");
      return;
    }

    // query port1 attribute. This is port id of the port to which the switch is connected.
    // If the controller does not have such a port, the message is disregarded.
    int rr_port1 = 0;
    if (element->QueryIntAttribute("port1", &rr_port1) != XML_SUCCESS) {
      mcLog("port1 attribute not found or wrong type. Message disregarded.");
      return;
    }
    mcLog("port1: " + String(rr_port1));
    if ((rr_port1 < 1 || rr_port1 > 32) && (rr_port1 < 1001 || rr_port1 > 2018)) {
      mcLog("Message disgarded, this controller does not have such a port.");
      return;
    }

    // query cmd attribute. This is the desired switch setting and can either be "turnout" or "straight".
    const char* rr_cmd = "-unknown-";
    if (element->QueryStringAttribute("cmd", &rr_cmd) != XML_SUCCESS) {
      mcLog("cmd attribute not found or wrong type.");
      return;
    }
    mcLog("cmd: " + String(rr_cmd));

    // query param1 attribute. This is the "straight" position of the switch servo motor.
    // defaults to SERVO_MIN
    int rr_param1 = SERVO_MIN;
    if (element->QueryIntAttribute("param1", &rr_param1) != XML_SUCCESS) {
      mcLog("param1 attribute not found or wrong type. Using default value.");
    }
    if (rr_param1 < SERVO_MIN_ALLOWED || rr_param1 > SERVO_MAX_ALLOWED) {
      // Reset angle back to standard if angle is too small
      // User has obviously forgotten to configure servo angle in Rocrail properly
      // To protect the servo, the default value is used
      mcLog("param1 attribute out of bounds. Using default value.");
      rr_param1 = SERVO_MIN;
    }
    mcLog("param1: " + String(rr_param1));

    // query value1 attribute. This is the "turnout" position of the switch servo motor.
    // defaults to SERVO_MAX
    int rr_value1 = SERVO_MAX;
    if (element->QueryIntAttribute("value1", &rr_value1) != XML_SUCCESS) {
      mcLog("value1 attribute not found or wrong type. Using default value.");
    }
    if (rr_value1 < SERVO_MIN_ALLOWED || rr_value1 > SERVO_MAX_ALLOWED) {
      // Reset angle back to standard if angle is too small
      // User has obviously forgotten to configure servo angle in Rocrail properly
      // To protect the servo, the default value is used
      mcLog("value1 attribute out of bounds. Using default value.");
      rr_value1 = SERVO_MAX;
    }
    mcLog("value1: " + String(rr_value1));

    // check command string and prepare servo angle
    // servo angle will only be used to flip a standard or one side of a triple switch - not for double slip switches!
    int switchCommand;
    int servoAngle;
    int servoAngle_2;
    int blink;
    if (strcmp(rr_cmd, "straight") == 0) {
      switchCommand = 1;
      servoAngle = rr_param1;
      servoAngle_2 = rr_param1;
      blink = HIGH;
      mcLog("Blink on");
    }
    else if (strcmp(rr_cmd, "turnout") == 0) {
      switchCommand = 0;
      servoAngle = rr_value1;
      servoAngle_2 = rr_value1+107;
      blink = LOW;
      mcLog("Blink off");
    }
    else {
      mcLog("Switch command unknown - message disregarded.");
      return;
    }

    for (int i = 0; i < NUM_PCA9685s; i++) {
      
      if (rr_port1 >= (i*10)+1001 && rr_port1 <= (i*10)+1008) {
        int servoPort1;
        int servoPort2;

        // port 1001 -> double slip switch 1 / side A / ports 1 and 2
        if (rr_port1 == (i*10)+1001) {
          servoPort1 = 1;
          servoPort2 = 2;  
        }
        // port 1002 -> double slip switch 1 / side B / ports 3 and 4
        else if (rr_port1 == (i*10)+1002) {
          servoPort1 = 3;
          servoPort2 = 4;
        }
        // port 1003 -> double slip switch 2 / side A / ports 5 and 6
        else if (rr_port1 == (i*10)+1003) {
          servoPort1 = 5;
          servoPort2 = 6;
        }
        // port 1004 -> double slip switch 2 / side B / ports 7 and 8
        else if (rr_port1 == (i*10)+1004) {
          servoPort1 = 7;
          servoPort2 = 8;
        }
        // port 1005 -> double slip switch 3 / side A / ports 1 and 2
        else if (rr_port1 == (i*10)+1005) {
          servoPort1 = 9;
          servoPort2 = 10;
        }
        // port 1006 -> double slip switch 3 / side B / ports 3 and 4
        else if (rr_port1 == (i*10)+1006) {
          servoPort1 = 11;
          servoPort2 = 12;
        }
        // port 1007 -> double slip switch 4 / side A / ports 5 and 6
        else if (rr_port1 == (i*10)+1007) {
          servoPort1 = 13;
          servoPort2 = 14;
        }
        // port 1008 -> double slip switch 4 / side B / ports 7 and 8
        else if (rr_port1 == (i*10)+1008) {
          servoPort1 = 15;
          servoPort2 = 16;
        }

        OUT3[rr_port1-((i*10)+1011)] = blink;                                                                //<-- edit here

        mcLog("Turning double slip switch servos on port " + String(servoPort1) + " and " + String(servoPort2) + " to angle " + String(servoAngle));
        setServoAngle(servoPort1 - 1, servoAngle, i);
        setServoAngle(servoPort2 - 1, servoAngle, i);
      }
      else if(rr_port1 >= 1 && rr_port1 <= 16) {
        mcLog("Turning servo on port " + String(rr_port1) + " to angle " + String(servoAngle));
        setServoAngle(rr_port1 -1, servoAngle, i);
      }
      else if(rr_port1 >= 17 && rr_port1 <= 128) {
        mcLog("Turning servo on port " + String(rr_port1-(16*i)) + " to angle " + String(servoAngle));
        setServoAngle((rr_port1-(16*i)) -1, servoAngle, i);
      }
      
    
    
    
    
    
    
      if (rr_port1 >= (i*10)+2001 && rr_port1 <= (i*10)+2004) {
      int servoPort1;
      int servoPort2;
      int servoPort3;
      int servoPort4;

      // port 2001 -> double slip switch 1 / side A / ports 1 2 3 4
      if (rr_port1 == (i*10)+2001) {
        servoPort1 = 1;
        servoPort2 = 2;  
        servoPort3 = 3;
        servoPort4 = 4;
      }
      // port 2002 -> double slip switch 2 / side A / ports 5 6 7 8
      else if (rr_port1 == 2002) {
        servoPort1 = 5;
        servoPort2 = 6;
        servoPort3 = 7;
        servoPort4 = 8;
      }
      // port 2003 -> double slip switch 3 / side A / ports 9 10 11 12
      else if (rr_port1 == (i*10)+2003) {
        servoPort1 = 9;
        servoPort2 = 10;
        servoPort3 = 11;
        servoPort4 = 12;
      }
      // port 2004 -> double slip switch 4 / side A / ports 13 14 15 16
      else if (rr_port1 == (i*10)+2004) {
        servoPort1 = 13;
        servoPort2 = 14;
        servoPort3 = 15;
        servoPort4 = 16;
      }

        OUT3[rr_port1-((i*10)+2001)] = blink;                                                                //<-- edit here
        

      mcLog("Turning double slip switch servos on port " + String(servoPort1) + " and " + String(servoPort2) + " to angle " + String(servoAngle));
      setServoAngle(servoPort1 - 1, servoAngle, i);
      setServoAngle(servoPort2 - 1, servoAngle, i);
      mcLog("Turning double slip switch servos on port " + String(servoPort3) + " and " + String(servoPort4) + " to angle " + String(servoAngle_2));
      setServoAngle(servoPort3 - 1, servoAngle_2, i);
      setServoAngle(servoPort4 - 1, servoAngle_2, i);
    }
    else if(rr_port1 >= 1 && rr_port1 <= 16) {
      mcLog("Turning servo on port " + String(rr_port1) + " to angle " + String(servoAngle));
      setServoAngle(rr_port1-1, servoAngle, i);
    }
    else if(rr_port1 >= 17 && rr_port1 <= 128) {
        mcLog("Turning servo on port " + String(rr_port1-(16*i)) + " to angle " + String(servoAngle));
        setServoAngle((rr_port1-(16*i)) -1, servoAngle, i);
      }
    
    
    }

      
    
    return;
    // end of switch command handling
  }




  element = xmlDocument.FirstChildElement("co");
  if (element != NULL) {
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
    

    // query cmd attribute. This is the desired switch setting and can either be "turnout" or "straight".
    const char* rr_cmd = "-unknown-";
    if (element->QueryStringAttribute("cmd", &rr_cmd) != XML_SUCCESS) {
      mcLog("cmd attribute not found or wrong type.");
      return;
    }
    mcLog("cmd: " + String(rr_cmd));

    // set signal LED for the port on/off
    if (strcmp(rr_cmd, "on") == 0) {
      setSignalLED(rr_port - 1, true);
    }
    else if (strcmp(rr_cmd, "off") == 0) {
      setSignalLED(rr_port - 1, false);
    }
    else {
      mcLog("Signal port command unknown - message disregarded.");
    }
    return;
    // end of signal handling
  }

  mcLog("No <sw> or <co> node found. Message disregarded.");
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
    //int sensorValue = PCF_03.read(SENSOR_PIN[i]);
    int b;
    int sensorValue;
    
    if(i <= 7)
    {
      b = i;
      sensorValue = PCF_05.read(IN[b]);                                                                 //<-- edit here
    }else if(i >= 8 && i <= 15){
      b = i - 8;
      sensorValue = PCF_06.read(IN[b]);                                                                 //<-- edit here
    }

    
    if (sensorValue == sensorTriggerState[i]) {
      // Contact -> report contact immediately
      if (!sensorState[i]) {
        mcLog("Sensor " + String(i) + " triggered.");
        sendSensorEvent2MQTT(i, true);
        sensorState[i] = true;
      }
      lastSensorContactMillis[i] = millis();
    }
    else {
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












// sets the servo arm to a desired angle
void setServoAngle(int servoIndex, int servoAngle, int PCAnr) {
  if (servoIndex >= 0 && servoIndex < NUM_SWITCHPORTS  || servoAngle >= SERVO_MIN && servoAngle <= SERVO_MAX) {
    setServoSleepMode(false, PCAnr);
    pca9685[PCAnr].setPWM(SWITCHPORT_PIN[servoIndex], 0, mapAngle2PulseLength(servoAngle));   
    delay(SWITCH_DELAY);
  }
  else {
    // this should not happen
    mcLog("WARNING_2: servo Angle " + String(servoAngle) + " out of range!");
    mcLog("WARNING_1: servo index " + String(servoIndex) + " out of range!");
  }
}





// converts a desired servo angle (0� - 180�) into a pwm pulse length (required for PCA9685)
int mapAngle2PulseLength(int angle) {
  const int PULSE_MIN = 0;
  const int PULSE_MAX = 600;
  return map(angle, 0, 180, PULSE_MIN, PULSE_MAX);
}

// Switches servo power supply on or off (presently supported for PCA9685 only)
void setServoSleepMode(bool onOff, int PCAnr) {
  if (PCA9685_OE_PIN_INSTALLED) {
    if (servoSleepMode != onOff) {
      digitalWrite(PCA9685_OE_PIN[PCAnr], onOff ? HIGH : LOW);                                                             
      servoSleepMode = onOff;
      if (onOff) {
        mcLog("Servo power turned off.");
      } else {
        mcLog("Servo power turned on.");
      }
    }
    if (!onOff) {
      servoSleepModeFrom_ms = millis() + SERVOSLEEPMODEAFTER_MS;
    }
  }
}

// Checks if servos can be set to sleep mode (presently supported for PCA9685 only)
void checkEnableServoSleepMode() {
  for (int i = 0; i < NUM_PCA9685s; i++) {
    if (millis() > servoSleepModeFrom_ms) {
      setServoSleepMode(true, i);
    }
  }
}

// switches a signal on or off
void setSignalLED(int signalIndex, bool ledState) {
  mcLog("Setting signal LED " + String(signalIndex) + " to " + String(ledState));
  
  if(signalIndex <= 7)
    {
      signalIndex = signalIndex - 0;
      PCF_01.write(OUT[signalIndex], ledState ? LOW : HIGH);
    }else if(signalIndex >= 8 && signalIndex <= 15){
      signalIndex = signalIndex - 8;
      PCF_03.write(OUT[signalIndex], ledState ? LOW : HIGH); 
    }
}


void corossingled() {                                                                                         //<-- edit here
  for (int crossingled = 0; crossingled < NUM_CROSSINGPORTS; ) {
    if(OUT3[crossingled] == HIGH){  
      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= interval) {
        // save the last time you blinked the LED
        previousMillis = currentMillis;
        // if the LED is off turn it on and vice-versa:
        if (OUT2[crossingled] == LOW) {
          OUT2[crossingled] = HIGH;
          OUT2[crossingled+1] = LOW;
        } else {
          OUT2[crossingled] = LOW;
          OUT2[crossingled+1] = HIGH;
        }

        // set the LED with the ledState of the variable:
        //digitalWrite(ledPin, OUT2[crossingled]);
        if(crossingled <= 7){
          crossingled = crossingled - 0;
          PCF_02.write(OUT[crossingled], OUT2[crossingled] ? LOW : HIGH); 
          PCF_02.write(OUT[crossingled+1], OUT2[crossingled+1] ? LOW : HIGH);         
        }else if(crossingled >= 8 && crossingled <= 15){
          crossingled = crossingled - 8;
          PCF_04.write(OUT[crossingled], OUT2[crossingled] ? LOW : HIGH); 
          PCF_04.write(OUT[crossingled+1], OUT2[crossingled+1] ? LOW : HIGH); 
        }
      }
    }else{
        if(crossingled <= 7){
          crossingled = crossingled - 0;
          PCF_02.write(OUT[crossingled], LOW ? LOW : HIGH); 
          PCF_02.write(OUT[crossingled+1], LOW ? LOW : HIGH);         
        }else if(crossingled >= 8 && crossingled <= 15){
          crossingled = crossingled - 8;
          PCF_04.write(OUT[crossingled], LOW ? LOW : HIGH); 
          PCF_04.write(OUT[crossingled+1], LOW ? LOW : HIGH);
        }
    }
    crossingled=crossingled+2;
  }    
}

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
   
#if ONLY_INPUTS
  monitorSensors();
#endif

#if ONLY_SIGNALS

#endif

#if ONLY_SWITCHES
  checkEnableServoSleepMode();
#endif

#if ONLY_CROSSINGS
  checkEnableServoSleepMode();
  corossingled();
  monitorSensors();
#endif
  
#if ONLY_BRIDGES

#endif
  
#if ONLY_ELEVATOR

#endif

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////END LOOP
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
