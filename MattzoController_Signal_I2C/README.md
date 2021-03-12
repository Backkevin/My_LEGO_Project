# MattzoController_Signal_I2C

-----------------------------------------------------------------

+ MattzoController_Signal_I2C
	+ V2.1
		* Work 100%
		* Partlist not OK
	+ Work !
	
-----------------------------------------------------------------

<img src="https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoController_Signal_I2C/IMAGE/CPU3.jpg">
<img src="https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoController_Signal_I2C/IMAGE/OUTPUT2.jpg">

-----------------------------------------------------------------

### Partslist OUTPUT
                    
  Part Name   |      PIC      |   Part Info            | Part Quantity |   Part Link 
------------- | ------------- | ---------------------- | ------------- | -------------
LED           |               | LED Rot 3mm            | 8             |
LED           |               | LED Grün 3mm           | 8             |
R             |               | 470 Ohm                | 32            |
Jumper        |               |                        |               |
C             |               | 100nF                  |               |
A,B           |               | Dip switch             | 2             |
R array       |               | 10K Wiederstandsarray  | 2             |
PCF8574P      |               | I/O                    | 2             |
ULN2803A      |               | Treiber                | 2             |
74HC540N      |               | Inverter               | 2             |
 
-----------------------------------------------------------------

### Partslist CPU
                    
  Part Name   |      PIC      |   Part Info            | Part Quantity |   Part Link 
------------- | ------------- | ---------------------- | ------------- | -------------


-----------------------------------------------------------------

 BUS PINOUT from CPU Board to other Boards
+ PIN 1 ---> D0
+ PIN 2 ---> SCL (D1)
+ PIN 3 ---> SDA (D2)
+ PIN 4 ---> D3
+ PIN 5 ---> D4
+ PIN 6 ---> D5
+ PIN 7 ---> D6
+ PIN 8 ---> D7
+ PIN 9 ---> D8
+ PIN 10 --> RX
+ PIN 11 --> TX
+ PIN 12 --> D9
+ PIN 13 --> +5VDC (Bug 3A/5A /\ AC/DC BUG 1A)
+ PIN 14 --> +5VDC
+ PIN 15 --> GND
+ PIN 16 --> GND

-----------------------------------------------------------------

+ PCF8574 adresse
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoController_Signal_I2C/IMAGE/PCF8574%20address%20map.png)
