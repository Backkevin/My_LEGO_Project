// MattzoSignalController Firmware
// Author: Dr. Matthias Runte
// Copyright 2020 by Dr. Matthias Runte
// License:
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Edited by DjPredator69 14/02/2021 tested = OK
// Software prepered for 8 x PCF8574 = 32 Signals
// NEED to check your adr of each PCF8574 and set the numbre of signal ports in config file
//
#define MATTZO_CONTROLLER_TYPE "MattzoSignalController"
#include <ESP8266WiFi.h>                          // WiFi library for ESP-8266
#include "MattzoSignalController_Configuration.h" // this file should be placed in the same folder
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


void setup() {
  // initialize signal pins
  /*
  for (int i = 0; i < NUM_SIGNALPORTS; i++) {
    pinMode(SIGNALPORT_PIN[i], OUTPUT);
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

  if(NUM_SIGNALPORTS > 8 && NUM_SIGNALPORTS < 16)
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

  if(NUM_SIGNALPORTS > 16 && NUM_SIGNALPORTS < 24)
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

  if(NUM_SIGNALPORTS > 24 && NUM_SIGNALPORTS < 32)
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

  if(NUM_SIGNALPORTS > 32 && NUM_SIGNALPORTS < 40)
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

  if(NUM_SIGNALPORTS > 40 && NUM_SIGNALPORTS < 48)
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

  if(NUM_SIGNALPORTS > 48 && NUM_SIGNALPORTS < 56)
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

  if(NUM_SIGNALPORTS > 56 )
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

void loop() {
  loopMattzoController();
}
