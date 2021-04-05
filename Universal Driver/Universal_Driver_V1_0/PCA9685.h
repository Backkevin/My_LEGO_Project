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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// ***************************
// Controller wiring specifics
// ***************************

// PCA9685 port expander used?
#define USE_PCA9685 false

// Number of chained PCA9685 port extenders
#define NUM_PCA9685s 1

// PCA9685 OE pin supported?
bool PCA9685_OE_PIN_INSTALLED = false;  // set to true if OE pin is connected (false if not) max 8 OE Pins
uint8_t PCA9685_OE_PIN[8]= {D3, D6 , D5, D7, D4, D9, D0, D8};  // D0,D3,D4,D5 right PCA D6,D7,D8,D9 left PCA D0,D8 info leds on cpu board

// Addresses of PCA9685 port extenders
uint8_t PCA9685_ADR[8]= {0x40, 0x41 , 0x42, 0x43, 0x44, 0x45, 0x46, 0x47}; //don`t edit
