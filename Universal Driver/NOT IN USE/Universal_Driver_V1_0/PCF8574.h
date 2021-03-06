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
//      Editable settings 
// ***************************

// PCF8574 port expander used?
#define USE_PCF8574 true

// Number of chained PCF8574 port extenders
#define NUM_PCF8574s 2

// Type of PCF8574 port extenders
#define TYPE_PCF8574 false // true = 20-27 or false = 38-3F



// ***************************
//     Controller settings 
// ***************************

// Number of sensors 
const int NUM_SENSORS = 8 * NUM_PCF8574s; //don`t edit
uint8_t IN[NUM_SENSORS] = {0, 1, 2, 3, 4, 5, 6, 7}; //don`t edit

// Number of signal ports
const int NUM_SIGNALPORTS = 8 * NUM_PCF8574s; //don`t edit
uint8_t OUT[NUM_SIGNALPORTS] = {0, 1, 2, 3, 4, 5, 6, 7}; //don`t edit

// Addresses of PCF8574 port extenders
#if TYPE_PCF8574
uint8_t PCF8574_ADR[8]= {0x20, 0x21 , 0x22, 0x23, 0x24, 0x25, 0x26, 0x27}; //don`t edit
#endif
#if !TYPE_PCF8574
uint8_t PCF8574_ADR[8]= {0x38, 0x39 , 0x38, 0x3A, 0x3B, 0x3C, 0x3E, 0x3F}; //don`t edit
#endif

// Number of signal ports for crossing
const int NUM_CROSSINGPORTS = 8;
uint8_t OUT2[16] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};// led state
uint8_t OUT3[16] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};// blink state
