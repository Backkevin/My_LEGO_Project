/*
* Switch V1.0 
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

// Number of switch ports
const int NUM_SWITCHPORTS = 32; 

// STATUS LED WIRING CONFIGURATION

// Digital output pin to monitor controller operation (typically a LED)
bool STATUS_LED_PIN_INSTALLED = true;  // set to false if no LED is installed
uint8_t STATUS_LED_PIN = D8; //D0 or D8 on CPU Board

//////////**********//////////

// PCA9685 port expander used?
#define USE_PCA9685 true

// Number of chained PCA9685 port extenders
#define NUM_PCA9685s 2

// PCA9685 OE pin supported?
bool PCA9685_OE_PIN_INSTALLED = true;  // set to true if OE pin is connected (false if not) max 8 OE Pins





// ****************
// Network settings
// ****************

// Trigger emergency brake upon disconnect
#define TRIGGER_EBREAK_UPON_DISCONNECT true


// ***************
// Syslog settings
// ***************

// Syslog application name
const char* SYSLOG_APP_NAME = "OSW";


// ***************************
//     Controller settings      //don`t edit
// ***************************

uint8_t PCA9685_OE_PIN[8]= {D3, D6 , D5, D7, D4, D9, D0, D8};  // D0,D3,D4,D5 right PCA D6,D7,D8,D9 left PCA D0,D8 info leds on cpu board

// Addresses of PCA9685 port extenders
uint8_t PCA9685_ADR[8]= {0x40, 0x41 , 0x42, 0x43, 0x44, 0x45, 0x46, 0x47};

// Digital output pins for switch servos
uint8_t SWITCHPORT_PIN[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
