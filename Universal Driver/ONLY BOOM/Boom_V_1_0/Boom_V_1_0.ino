/*
 * PCA9685 written but not tested
 */

#define MATTZO_CONTROLLER_TYPE "OB"


#include <ESP8266WiFi.h>                          // WiFi library for ESP-8266
#include <Wire.h>                                 // Built-in library for I2C
#include "Config.h"                               // this file should be placed in the same folder
#include "MattzoController_Library.h"             // this file needs to be placed in the Arduino library folder


#include <Adafruit_PWMServoDriver.h>              // Adafruit PWM Servo Driver Library for PCA9685 port expander. Tested with version 2.4.0.
Adafruit_PWMServoDriver pca9685[NUM_PCA9685s];
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates


// ******************
// BOOM settings
// ******************

// Default values for TrixBrix boomes (in case servo angles are not transmitted)
const int SERVO_MIN_ALLOWED = 30;   // minimum accepted servo angle from Rocrail. Anything below this value is treated as misconfiguration and is neglected and reset to SERVO_MIN.
const int SERVO_MIN = 79;           // a good first guess for the minimum angle of TrixBrix servos is 79
const int SERVO_START = 83;         // position after boot-up. For TrixBrix servos, this is more or less the middle position
const int SERVO_MAX = 87;           // a good first guess for the maximum angle of TrixBrix servos is 87
const int SERVO_MAX_ALLOWED = 100;  // maximum accepted servo angle from Rocrail. Anything above this value is treated as misconfiguration and is neglected and reset to SERVO_MAX.

// Delay between two boom operations
const int BOOM_DELAY = 200;

// Time after servo operation until servo power is switched off (in milliseconds; 3000 = 3 sec.)
// Presently only supported when using PCA9685
const int SERVOSLEEPMODEAFTER_MS = 500;

// time when servos will go to sleep mode
bool servoSleepMode = false;
unsigned long servoSleepModeFrom_ms = 100;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////START SETUP
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
   
Wire.begin();
  
delay(500);
  
// load config from EEPROM, initialize Wifi, MQTT etc.
setupMattzoController();

delay(500);

mcLog(" Configured as BOOM"); 

setupPCA9685();

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////END SETUP
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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

    // query port1 attribute. This is port id of the port to which the boom is connected.
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

    // query cmd attribute. This is the desired boom setting and can either be "turnout" or "straight".
    const char* rr_cmd = "-unknown-";
    if (element->QueryStringAttribute("cmd", &rr_cmd) != XML_SUCCESS) {
      mcLog("cmd attribute not found or wrong type.");
      return;
    }
    mcLog("cmd: " + String(rr_cmd));

    // query param1 attribute. This is the "straight" position of the boom servo motor.
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

    // query value1 attribute. This is the "turnout" position of the boom servo motor.
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
    // servo angle will only be used to flip a standard or one side of a triple boom - not for double slip boomes!
    int boomCommand;
    int servoAngle;
    if (strcmp(rr_cmd, "straight") == 0) {
      boomCommand = 1;
      servoAngle = rr_param1;
    }
    else if (strcmp(rr_cmd, "turnout") == 0) {
      boomCommand = 0;
      servoAngle = rr_value1;
    }
    else {
      mcLog("boom command unknown - message disregarded.");
      return;
    }



    for (int i = 0; i < NUM_PCA9685s; i++) {
      
      if (rr_port1 >= (i*10)+1001 && rr_port1 <= (i*10)+1008) {
        int servoPort1;
        int servoPort2;

        // port 1001 -> double slip boom 1 / side A / ports 1 and 2
        if (rr_port1 == (i*10)+1001) {
          servoPort1 = 1;
          servoPort2 = 2;  
        }
        // port 1002 -> double slip boom 1 / side B / ports 3 and 4
        else if (rr_port1 == (i*10)+1002) {
          servoPort1 = 3;
          servoPort2 = 4;
        }
        // port 1003 -> double slip boom 2 / side A / ports 5 and 6
        else if (rr_port1 == (i*10)+1003) {
          servoPort1 = 5;
          servoPort2 = 6;
        }
        // port 1004 -> double slip boom 2 / side B / ports 7 and 8
        else if (rr_port1 == (i*10)+1004) {
          servoPort1 = 7;
          servoPort2 = 8;
        }
        // port 1005 -> double slip boom 3 / side A / ports 1 and 2
        else if (rr_port1 == (i*10)+1005) {
          servoPort1 = 9;
          servoPort2 = 10;
        }
        // port 1006 -> double slip boom 3 / side B / ports 3 and 4
        else if (rr_port1 == (i*10)+1006) {
          servoPort1 = 11;
          servoPort2 = 12;
        }
        // port 1007 -> double slip boom 4 / side A / ports 5 and 6
        else if (rr_port1 == (i*10)+1007) {
          servoPort1 = 13;
          servoPort2 = 14;
        }
        // port 1008 -> double slip boom 4 / side B / ports 7 and 8
        else if (rr_port1 == (i*10)+1008) {
          servoPort1 = 15;
          servoPort2 = 16;
        }

        mcLog("Turning double slip boom servos on port " + String(servoPort1) + " and " + String(servoPort2) + " to angle " + String(servoAngle));
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
    // end of boom command handling
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

    // query port attribute. This is port id of the port to which the boom is connected.
    // If the controller does not have such a port, the message is disregarded.
    int rr_port = 0;
    if (element->QueryIntAttribute("port", &rr_port) != XML_SUCCESS) {
      mcLog("port attribute not found or wrong type. Message disregarded.");
      return;
    }
    mcLog("port: " + String(rr_port));
    

    // query cmd attribute. This is the desired boom setting and can either be "turnout" or "straight".
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
  if (servoIndex >= 0 && servoIndex < NUM_BOOMPORTS) {
    setServoSleepMode(false, PCAnr);
    pca9685[PCAnr].setPWM(BOOMPORT_PIN[servoIndex], 0, mapAngle2PulseLength(servoAngle));   
    delay(BOOM_DELAY);
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








// boomes servo power supply on or off (presently supported for PCA9685 only)
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////START LOOP
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  
  loopMattzoController();
  checkEnableServoSleepMode();
  
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////END LOOP
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
