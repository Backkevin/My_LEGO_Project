// MattzoSwitchController Firmware
// Author: Dr. Matthias Runte
// Copyright 2020 by Dr. Matthias Runte
// License:
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#define MATTZO_CONTROLLER_TYPE "MattzoSwitchController"
#include <ESP8266WiFi.h>                          // WiFi library for ESP-8266
//#include <Servo.h>                                // Servo library
#include "MattzoSwitchController_Configuration.h" // this file should be placed in the same folder
#include "MattzoController_Library.h"             // this file needs to be placed in the Arduino library folder

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
double pulselength;


// Create servo objects to control servos
//Servo servo[NUM_SWITCHPORTS];

// Default values for TrixBrix switches (in case servo angles are not transmitted)
const int SERVO_MIN_ALLOWED = 50;   // minimum accepted servo angle from Rocrail. Anything below this value is treated as misconfiguration and is neglected and reset to SERVO_MIN.
const int SERVO_MIN = 75;           // a good first guess for the minimum angle of TrixBrix servos is 70
const int SERVO_START = 80;         // position after boot-up. For TrixBrix servos, this is more or less the middle position
const int SERVO_MAX = 85;           // a good first guess for the maximum angle of TrixBrix servos is 90
const int SERVO_MAX_ALLOWED = 120;  // maximum accepted servo angle from Rocrail. Anything above this value is treated as misconfiguration and is neglected and reset to SERVO_MAX.

// Delay between two switch operations
const int SWITCH_DELAY = 200;



void setup() {
    // initialize servo pins and turn servos to start position
    /*
    for (int i = 0; i < NUM_SWITCHPORTS; i++) {
      servo[i].attach(SWITCHPORT_PIN[i]);
      servo[i].write(SERVO_START);
      delay(SWITCH_DELAY);
    }
    */

    pwm1.begin();
    pwm1.setOscillatorFrequency(27000000);
    pwm1.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
    pwm2.begin();
    pwm2.setOscillatorFrequency(27000000);
    pwm2.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
    

    // load config from EEPROM, initialize Wifi, MQTT etc.
    setupMattzoController();
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  char msg[length + 1];
  for (int i = 0; i < length; i++) {
    msg[i] = (char)payload[i];
  }
  msg[length] = '\0';

  mcLog("Received MQTT message [" + String(topic) + "]: " + String(msg));

  XMLDocument xmlDocument;
  if(xmlDocument.Parse(msg)!= XML_SUCCESS){
    mcLog("Error parsing XML.");
    return;
  }
  mcLog("Parsing XML successful.");

  XMLElement * element = xmlDocument.FirstChildElement("sw");
  if (element == NULL) {
    mcLog("<sw> node not found. Message disregarded.");
    return;
  }

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
  if ((rr_port1 < 1 || rr_port1 > NUM_SWITCHPORTS) && (rr_port1 < 1001 || rr_port1 > 1004)) {
    mcLog("Message disgarded, this controller does not have such a port.");
    return;
  }

  // query cmd attribute. This is the desired switch setting and can either be "turnout" or "straight".
  const char * rr_cmd = "-unknown-";
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
  if (strcmp(rr_cmd, "straight")==0) {
    switchCommand = 1;
    servoAngle = rr_param1;
  } else if (strcmp(rr_cmd, "turnout")==0) {
    switchCommand = 0;
    servoAngle = rr_value1;
  } else {
    mcLog("Switch command unknown - message disregarded.");
    return;
  }

  //PWM1
  // Check if port is between 1001 and 1016 -> special rules for trixbrix double slip switches apply!
  if (rr_port1 >= 1001 && rr_port1 <= 1016) {
    int servoPort1;
    int servoPort2;

    // port 1001 -> double slip switch 1 / side A / ports 1 and 2
    if (rr_port1 == 1001) {
      servoPort1 = 1;
      servoPort2 = 2;
    // port 1002 -> double slip switch 1 / side B / ports 3 and 4
    } else if (rr_port1 == 1002) {
      servoPort1 = 3;
      servoPort2 = 4;
    // port 1003 -> double slip switch 2 / side A / ports 5 and 6
    } else if (rr_port1 == 1003) {
      servoPort1 = 5;
      servoPort2 = 6;
    // port 1004 -> double slip switch 2 / side B / ports 7 and 8
    } else if (rr_port1 == 1004) {
      servoPort1 = 7;
      servoPort2 = 8;
    // port 1005 -> double slip switch 3 / side A / ports 9 and 10
    } else if (rr_port1 == 1005) {
      servoPort1 = 9;
      servoPort2 = 10;
    // port 1006 -> double slip switch 3 / side B / ports 11 and 12
    } else if (rr_port1 == 1006) {
      servoPort1 = 11;
      servoPort2 = 12;
    // port 1007 -> double slip switch 4 / side A / ports 13 and 14
    } else if (rr_port1 == 1007) {
      servoPort1 = 13;
      servoPort2 = 14;
    // port 1008 -> double slip switch 4 / side B / ports 15 and 16
    } else if (rr_port1 == 1008) {
      servoPort1 = 15;
      servoPort2 = 16;
    }

    //PWM2
    // port 1009 -> double slip switch 3 / side A / ports 9 and 10
    else if (rr_port1 == 1009) {
      servoPort1 = 17;
      servoPort2 = 18;
    // port 1010 -> double slip switch 3 / side B / ports 11 and 12
    } else if (rr_port1 == 1010) {
      servoPort1 = 19;
      servoPort2 = 20;
    // port 1011 -> double slip switch 4 / side A / ports 13 and 14
    } else if (rr_port1 == 1011) {
      servoPort1 = 21;
      servoPort2 = 22;
    // port 1012 -> double slip switch 4 / side B / ports 15 and 16
    } else if (rr_port1 == 1012) {
      servoPort1 = 23;
      servoPort2 = 24;
    // port 1013 -> double slip switch 3 / side A / ports 9 and 10
    } else if (rr_port1 == 1013) {
      servoPort1 = 25;
      servoPort2 = 26;
    // port 1014 -> double slip switch 3 / side B / ports 11 and 12
    } else if (rr_port1 == 1014) {
      servoPort1 = 27;
      servoPort2 = 28;
    // port 1015 -> double slip switch 4 / side A / ports 13 and 14
    } else if (rr_port1 == 1015) {
      servoPort1 = 29;
      servoPort2 = 30;
    // port 1016 -> double slip switch 4 / side B / ports 15 and 16
    } else if (rr_port1 == 1016) {
      servoPort1 = 31;
      servoPort2 = 32;
    }
    

    mcLog("Turning double slip switch servos on port " + String(servoPort1) + " and " + String(servoPort2) + " to angle " + String(servoAngle));
    setServoAngle(servoPort1 - 1, servoAngle);
    setServoAngle(servoPort2 - 1, servoAngle);
  }
  else {
    mcLog("Turning servo on port " + String(rr_port1) + " to angle " + String(servoAngle));
    setServoAngle(rr_port1 - 1, servoAngle);
  }
}

void setServoAngle(int servoIndex, int servoAngle) {
  if (servoIndex >= 0 && servoIndex < 17) {
    //servo[servoIndex].write(servoAngle);
    pulselength = map(servoAngle, SERVO_MIN_ALLOWED, SERVO_MAX_ALLOWED, SERVOMIN, SERVOMAX);
    pwm1.setPWM(servoIndex, 0, pulselength);
    delay(SWITCH_DELAY);
  }
  else if (servoIndex >= 17 && servoIndex < NUM_SWITCHPORTS) {
    //servo[servoIndex].write(servoAngle);
    servoIndex = servoIndex - 16;
    pulselength = map(servoAngle, SERVO_MIN_ALLOWED, SERVO_MAX_ALLOWED, SERVOMIN, SERVOMAX);
    pwm2.setPWM(servoIndex, 0, pulselength);
    delay(SWITCH_DELAY);
  }
  else {
    // this should not happen
    mcLog("WARNING: servo index " + String(servoIndex) + " out of range!");
  }
}

void loop() {
  loopMattzoController();
}
