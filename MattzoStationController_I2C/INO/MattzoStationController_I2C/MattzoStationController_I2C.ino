// MattzoSwitchController Firmware
// Author: Dr. Matthias Runte
// Copyright 2020 by Dr. Matthias Runte
// License:
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#define MATTZO_CONTROLLER_TYPE "MattzoStationController"
#include <ESP8266WiFi.h>                          // WiFi library for ESP-8266
#include <Servo.h>                                // Servo library
#include "MattzoStationController_Configuration.h" // this file should be placed in the same folder
#include "MattzoController_Library.h"             // this file needs to be placed in the Arduino library folder

#if USE_PCA9685
#include <Wire.h>                                 // Built-in library for I2C
#include <Adafruit_PWMServoDriver.h>              // Adafruit PWM Servo Driver Library for PCA9685 port expander. Tested with version 2.4.0.
#endif

#include <PCF8574.h>                              //
PCF8574 PCF_01(0x38);                             // set adr from PCF8574   ==> 000
PCF8574 PCF_02(0x39);                             // set adr from PCF8574   ==> 100
PCF8574 PCF_03(0x3A);                             // set adr from PCF8574   ==> 010
PCF8574 PCF_04(0x3B);                             // set adr from PCF8574   ==> 110


// SERVO VARIABLES AND CONSTANTS

// Create servo objects to control servos
Servo servo[NUM_SWITCHPORTS];

// Create PWM Servo Driver object (for PCA9685)
#if USE_PCA9685
Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver();  // only board 0x40 is supported in this version of the firmware
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
#endif


// Default values for TrixBrix switches (in case servo angles are not transmitted)
const int SERVO_MIN_ALLOWED = 30;   // minimum accepted servo angle from Rocrail. Anything below this value is treated as misconfiguration and is neglected and reset to SERVO_MIN.
const int SERVO_MIN = 35;           // a good first guess for the minimum angle of TrixBrix servos is 70
const int SERVO_START = 65;         // position after boot-up. For TrixBrix servos, this is more or less the middle position
const int SERVO_MAX = 90;           // a good first guess for the maximum angle of TrixBrix servos is 90
const int SERVO_MAX_ALLOWED = 100;  // maximum accepted servo angle from Rocrail. Anything above this value is treated as misconfiguration and is neglected and reset to SERVO_MAX.

// Delay between two switch operations
const int SWITCH_DELAY = 200;

// Time after servo operation until servo power is switched off (in milliseconds; 3000 = 3 sec.)
// Presently only supported when using PCA9685
const int SERVOSLEEPMODEAFTER_MS = 3000;

// time when servos will go to sleep mode
bool servoSleepMode = false;
unsigned long servoSleepModeFrom_ms = 0;


// SENSOR VARIABLES AND CONSTANTS

// Time in milliseconds until release event is reported after sensor has lost contact
const int SENSOR_RELEASE_TICKS = 100;
bool sensorState[NUM_SENSORS];
int sensorTriggerState[NUM_SENSORS];
int lastSensorContactMillis[NUM_SENSORS];



void setup() {

  
  
  // initialize PWM Servo Driver object (for PCA9685)
#if USE_PCA9685
  setupPCA9685();

  // Switch PCA9685 off
  if (PCA9685_OE_PIN_INSTALLED) {
    pinMode(PCA9685_OE_PIN, OUTPUT);
    setServoSleepMode(true);
  }
#endif

  // initialize servo pins and turn servos to start position
  for (int i = 0; i < NUM_SWITCHPORTS; i++) {
    if (SWITCHPORT_PIN_TYPE[i] == 0) {
      // servo connected directly to the controller
      servo[i].attach(SWITCHPORT_PIN[i]);
      servo[i].write(SERVO_START);
      delay(SWITCH_DELAY);
    }
    else if (SWITCHPORT_PIN_TYPE[i] == 0x40) {
      // servo connected to PCA9685
      // no action required
    }
  }

  

  // initialize sensor pins
  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(SENSOR_PIN[i], INPUT_PULLUP);
    sensorState[i] = false;
    sensorTriggerState[i] = (SENSOR_PIN[i] == D8) ? HIGH : LOW;
  }


  // load config from EEPROM, initialize Wifi, MQTT etc.
  setupMattzoController();

  Wire.begin();

  PCF_01.begin();
  
  if (PCF_01.isConnected())
  {
    mcLog("=>(1) connected");

  }
  PCF_02.begin();
  
  if (PCF_02.isConnected())
  {
    mcLog("=>(2) connected");

  }


}

#if USE_PCA9685
void setupPCA9685() {
  // Initialize PWM Servo Driver object (for PCA9685)
  pca9685.begin();
  pca9685.setOscillatorFrequency(27000000);
  pca9685.setPWMFreq(SERVO_FREQ);
  delay(10);
}
#endif

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
    if ((rr_port1 < 1 || rr_port1 > NUM_SWITCHPORTS) && (rr_port1 < 1001 || rr_port1 > 1004)) {
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

    // Check if port is between 1001 and 1004 -> special rules for trixbrix double slip switches apply!
    if (rr_port1 >= 1001 && rr_port1 <= 1004) {
      int servoPort1;
      int servoPort2;

      // port 1001 -> double slip switch 1 / side A / ports 1 and 2
      if (rr_port1 == 1001) {
        servoPort1 = 1;
        servoPort2 = 2;
        // port 1002 -> double slip switch 1 / side B / ports 3 and 4
      }
      else if (rr_port1 == 1002) {
        servoPort1 = 3;
        servoPort2 = 4;
        // port 1003 -> double slip switch 2 / side A / ports 5 and 6
      }
      else if (rr_port1 == 1003) {
        servoPort1 = 5;
        servoPort2 = 6;
        // port 1004 -> double slip switch 2 / side B / ports 7 and 8
      }
      else if (rr_port1 == 1004) {
        servoPort1 = 7;
        servoPort2 = 8;
      }

      mcLog("Turning double slip switch servos on port " + String(servoPort1) + " and " + String(servoPort2) + " to angle " + String(servoAngle));
      setServoAngle(servoPort1 - 1, servoAngle);
      setServoAngle(servoPort2 - 1, servoAngle);
    }
    else {
      mcLog("Turning servo on port " + String(rr_port1) + " to angle " + String(servoAngle));
      setServoAngle(rr_port1 - 1, servoAngle);
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
    if (rr_port < 1 || rr_port > NUM_SIGNALPORTS) {
      mcLog("Message disgarded, as this controller does not have such a port.");
      return;
    }

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
    int sensorValue = digitalRead(SENSOR_PIN[i]);

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
void setServoAngle(int servoIndex, int servoAngle) {
  if (servoIndex >= 0 && servoIndex < NUM_SWITCHPORTS) {
    if (SWITCHPORT_PIN_TYPE[servoIndex] == 0) {
      servo[servoIndex].write(servoAngle);
    }
#if USE_PCA9685
    else if (SWITCHPORT_PIN_TYPE[servoIndex] == 0x40) {
      setServoSleepMode(false);
      pca9685.setPWM(SWITCHPORT_PIN[servoIndex], 0, mapAngle2PulseLength(servoAngle));
    }
#endif
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
void setServoSleepMode(bool onOff) {
  if (PCA9685_OE_PIN_INSTALLED) {
    if (servoSleepMode != onOff) {
      digitalWrite(PCA9685_OE_PIN, onOff ? HIGH : LOW);
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
  if (millis() > servoSleepModeFrom_ms) {
    setServoSleepMode(true);
  }
}

// switches a signal on or off
void setSignalLED(int signalIndex, bool ledState) {
  mcLog("Setting signal LED " + String(signalIndex) + " to " + String(ledState));
  
    PCF_01.write(SIGNALPORT_PIN[signalIndex], ledState ? LOW : HIGH);
  
}

void loop() {
  loopMattzoController();
  checkEnableServoSleepMode();
  monitorSensors();
}
