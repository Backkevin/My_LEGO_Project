/*
 * PCF8574 written but not tested
 */

#define MATTZO_CONTROLLER_TYPE "OOUT"


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
#include "Adafruit_MCP23017.h"
Adafruit_MCP23017 mcp23017[NUM_MCP23017s];
#endif


// ******************
// OUTPUT settings
// ******************



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
  for (int m = 0; m < NUM_MCP23017s; m++) {
    mcp23017[m] = Adafruit_MCP23017();
    mcp23017[m].begin();

    for (int i = 0; i < 15; i++) {
      mcp23017[m].pinMode(OUT_MCP[i], OUTPUT);
    }
  }
}
#endif


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

#if USE_MCP23017

    if(index <= 15)
    {
      index = index - 0;
      mcp23017[0].digitalWrite(OUT_MCP[index], ledState ? LOW : HIGH);
    }else if(index >= 16 && index <= 31){
      index = index - 16;
      mcp23017[1].digitalWrite(OUT_MCP[index], ledState ? LOW : HIGH); 
    }else if(index >= 32 && index <= 47){
      index = index - 32;
      mcp23017[2].digitalWrite(OUT_MCP[index], ledState ? LOW : HIGH);
    }else if(index >= 48 && index <= 63){
      index = index - 48;
      mcp23017[3].digitalWrite(OUT_MCP[index], ledState ? LOW : HIGH);
    }else if(index >= 64 && index <= 79){
      index = index - 64;
      mcp23017[4].digitalWrite(OUT_MCP[index], ledState ? LOW : HIGH);
    }else if(index >= 80 && index <= 95){
      index = index - 80;
      mcp23017[5].digitalWrite(OUT_MCP[index], ledState ? LOW : HIGH);
    }else if(index >= 96 && index <= 111){
      index = index - 96;
      mcp23017[6].digitalWrite(OUT_MCP[index], ledState ? LOW : HIGH);
    }else if(index >= 112){
      index = index - 112;
      mcp23017[7].digitalWrite(OUT_MCP[index], ledState ? LOW : HIGH);
    }

#endif

#if USE_PCF8574
  
  if(index <= 7)
    {
      index = index - 0;
      PCF_01.write(OUT_PCF[index], ledState ? LOW : HIGH);
    }else if(index >= 8 && index <= 15){
      index = index - 8;
      PCF_02.write(OUT_PCF[index], ledState ? LOW : HIGH); 
    }else if(index >= 16 && index <= 23){
      index = index - 16;
      PCF_03.write(OUT_PCF[index], ledState ? LOW : HIGH);
    }else if(index >= 24 && index <= 31){
      index = index - 24;
      PCF_04.write(OUT_PCF[index], ledState ? LOW : HIGH);
    }else if(index >= 32 && index <= 39){
      index = index - 32;
      PCF_05.write(OUT_PCF[index], ledState ? LOW : HIGH);
    }else if(index >= 40 && index <= 47){
      index = index - 40;
      PCF_06.write(OUT_PCF[index], ledState ? LOW : HIGH);
    }else if(index >= 48 && index <= 55){
      index = index - 48;
      PCF_07.write(OUT_PCF[index], ledState ? LOW : HIGH);
    }else if(index >= 56){
      index = index - 56;
      PCF_08.write(OUT_PCF[index], ledState ? LOW : HIGH);
    }

#endif
    
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////START LOOP
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  
  loopMattzoController();
  
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////END LOOP
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
