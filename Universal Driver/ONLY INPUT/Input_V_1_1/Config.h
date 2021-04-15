/*
* Input V1.0 
* 
* Author: BACK KEVIN
* Copyright 2021 by Back Kevin
* License:
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
* 
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// ***************************
//      Editable settings 
// ***************************

// Number of sensors  (PCF8574 max 8x8=64 ,MCP32017 max 8x16=128)
const int NUM_SENSORS = 16; 

// STATUS LED WIRING CONFIGURATION

// Digital output pin to monitor controller operation (typically a LED)
bool STATUS_LED_PIN_INSTALLED = true;  // set to false if no LED is installed
uint8_t STATUS_LED_PIN = D8; //D0 or D8 on CPU Board

//////////**********//////////

// PCF8574 port expander used?
#define USE_PCF8574 false

// Number of chained PCF8574 port extenders  (max 8)
#define NUM_PCF8574s 2

// Type of PCF8574 port extenders
#define TYPE_PCF8574 false // true = 20-27 or false = 38-3F

//////////****AND****////////// if you use both PCF8574s are the first then MCP23017

// MCP23017 port expander used?
#define USE_MCP23017  true

// Number of chained PCA9685 port extenders (max 8)
#define NUM_MCP23017s 1


// ****************
// Network settings
// ****************

// Trigger emergency brake upon disconnect
#define TRIGGER_EBREAK_UPON_DISCONNECT true


// ***************
// Syslog settings
// ***************

// Syslog application name
const char* SYSLOG_APP_NAME = "OIN";


// ***************************
//     Controller settings      //don`t edit
// ***************************


uint8_t IN_PCF[8] = {0, 1, 2, 3, 4, 5, 6, 7}; 

// Addresses of PCF8574 port extenders
#if TYPE_PCF8574
uint8_t PCF8574_ADR[8]= {0x20, 0x21 , 0x22, 0x23, 0x24, 0x25, 0x26, 0x27};
#else
uint8_t PCF8574_ADR[8]= {0x38, 0x39 , 0x38, 0x3A, 0x3B, 0x3C, 0x3E, 0x3F}; 
#endif

//////////**********//////////

uint8_t IN_MCP[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}; 
