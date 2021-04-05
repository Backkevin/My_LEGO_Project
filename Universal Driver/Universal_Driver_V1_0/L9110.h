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

// L9110_BOARD used?
#define USE_L9110_BOARD false

// Number of chained L9110_BOARD
#define NUM_L9110_BOARDs 1






// ***************************
//     Controller settings 
// ***************************

// Addresses of MCP23017 port extenders on L9110_BOARD
uint8_t L9110_BOARD_ADR[8]= {0x27, 0x26 , 0x25, 0x24, 0x23, 0x22, 0x21, 0x20}; //don`t edit
