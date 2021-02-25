// MattzoController Network Configuration
// Author: Dr. Matthias Runte
// Copyright 2020 by Dr. Matthias Runte
// License:
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// This file needs to stay in the folder of the firmware for the specific MattzoController!

// Handling of different configurations:
//		It's a good idea to create copies for all the controllers of this type that you own.
//		Before compiling and uploading the firmware, just include the correct configuration into the firmware code.
//		This allows to manage specific configurations for the different controllers easily.

// Best practice:
// 1. Create a copy of this file if required (see above).
// 2. Go through the settings below and update the settings as required.



// ***************************
// Controller wiring specifics
// ***************************

// Number of signal ports
const int NUM_SIGNALPORTS = 16;    //val=8   ==> 1 PCF8574 or val=16  ==> 2 PCF8574 
                                  //val=24  ==> 3 PCF8574 or val=32  ==> 4 PCF8574
                                  //val=40  ==> 5 PCF8574 or val=48  ==> 6 PCF8574
                                  //val=56  ==> 7 PCF8574 or val=64  ==> 8 PCF8574

// Digital output pin to monitor controller operation (typically a LED)
bool STATUS_LED_PIN_INSTALLED = true;  // set to false if no LED is installed
uint8_t STATUS_LED_PIN = D0;  // pin of the status LED (only relevant if installed)


// ****************
// PCF8574 settings
// ****************

uint8_t OUT[NUM_SIGNALPORTS] = {0, 1, 2, 3, 4, 5, 6, 7};


// ****************
// Network settings
// ****************

// Trigger emergency brake upon disconnect
#define TRIGGER_EBREAK_UPON_DISCONNECT false


// ***************
// Syslog settings
// ***************

// Syslog application name
const char* SYSLOG_APP_NAME = "MSIC";
