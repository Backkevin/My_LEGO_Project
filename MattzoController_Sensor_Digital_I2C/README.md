# MattzoController_Sensor_Digital_I2C

-----------------------------------------------------------------

+ MattzoController_Sensor_Digital_I2C
	+ V2.1
		* Work 90%
		* Partlist not OK
	+ Work in progress !

-----------------------------------------------------------------

<img src="https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoController_Sensor_Digital_I2C/IMAGE/Input%20pinout.jpg">

<img src="https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoController_Sensor_Digital_I2C/IMAGE/CPU3.jpg">
<img src="https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoController_Sensor_Digital_I2C/IMAGE/INPUT2.jpg">

-----------------------------------------------------------------

### Partslist INPUT
                    
  Part Name   |      PIC      |   Part Info            | Part Quantity |   Part Link 
------------- | ------------- | ---------------------- | ------------- | -------------
LED           |               | LED 5 mm               | 16            |
R             |               | 470 Ohm                | 16            |
R             |               | 10K Ohm                | 16            |
Jumper        |               |                        |               |
C             |               | 100nF                  |               |
PCF8574P      |               | I/O                    | 2             |

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
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoController_Sensor_Digital_I2C/IMAGE/PCF8574%20address%20map.png)
