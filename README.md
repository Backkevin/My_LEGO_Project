# My_LEGO_Project

based on MattzoController Firmware V0.3

 My Lego boards with PCF8574 ,MC23017 ,PCA9685 ,L9110
 
 https://www.pcbway.com/setinvite.aspx?inviteid=295257
 
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
+ PIN 13 --> +5VDC (Bug 3A/5A /\ AC/DC BUG 1A)   Layouts are not ready for the 5A BUG (this bug is bigger, change in the next layout)
+ PIN 14 --> +5VDC (Bug 3A/5A /\ AC/DC BUG 1A)   Layouts are not ready for the 5A BUG (this bug is bigger, change in the next layout)
+ PIN 15 --> GND
+ PIN 16 --> GND

-----------------------------------------------------------------

+ PCF8574 adresse
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoController_Sensor_Digital_I2C/IMAGE/PCF8574%20address%20map.png)
+ MCP23017 adresse
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MCP23017%20Boards/I16/IMAGE/MCP23017-I2C-Address.png)
+ PCA9685 adresse
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoStationController_I2C/IMAGE/pca9685-adressen.jpg)
-----------------------------------------------------------------

+ Conpatible sensors and actors
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoController_Sensor_Digital_I2C/IMAGE/train_sensor.jpg)
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoController_Signal_I2C/IMAGE/train_light.jpg)
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoStationController_I2C/IMAGE/train_light2.jpg)
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoController_Switch_I2C/IMAGE/train_switch.jpg)
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoStationController_I2C/IMAGE/train_boom.jpg)
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoStationController_I2C/IMAGE/A3144.jpg)
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoStationController_I2C/IMAGE/magnet.jpg)
>![]()
-----------------------------------------------------------------
-----------------------------------------------------------------

+ [MattzoController_Sensor_Digital_I2C](https://github.com/Backkevin/My_LEGO_Project/tree/master/MattzoController_Sensor_Digital_I2C)

	+ V2.1
		* Work 90%
		* Partlist KO
		* [INO FILE](https://github.com/Backkevin/My_LEGO_Project/tree/master/MattzoController_Sensor_Digital_I2C/INO/MattzoController_Sensor_Digital_I2C)
		* [GERBER FILES](https://github.com/Backkevin/My_LEGO_Project/tree/master/MattzoController_Sensor_Digital_I2C/GERBER%20FILES)
		* [EAGLE FILES](https://github.com/Backkevin/My_LEGO_Project/tree/master/MattzoController_Sensor_Digital_I2C/EAGLE)
		* [PDF FILES](https://github.com/Backkevin/My_LEGO_Project/tree/master/MattzoController_Sensor_Digital_I2C/PDF)
		
	+ Work in progress !
>+ CPU V3.0
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoController_Sensor_Digital_I2C/IMAGE/CPU3.jpg)
>+ INPUT V2.1
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoController_Sensor_Digital_I2C/IMAGE/INPUT2.jpg)
-----------------------------------------------------------------


+ [MattzoController_Signal_I2C](https://github.com/Backkevin/My_LEGO_Project/tree/master/MattzoController_Signal_I2C)

	+ V2.1
		* Work 100%
		* Partlist KO
		* [INO FILE](https://github.com/Backkevin/My_LEGO_Project/tree/master/MattzoController_Signal_I2C/INO/MattzoController_Signal_I2C)
		* [GERBER FILES](https://github.com/Backkevin/My_LEGO_Project/tree/master/MattzoController_Signal_I2C/GERBER%20FILES)
		* [EAGLE FILES](https://github.com/Backkevin/My_LEGO_Project/tree/master/MattzoController_Signal_I2C/EAGLE)
		* [PDF FILES](https://github.com/Backkevin/My_LEGO_Project/tree/master/MattzoController_Signal_I2C/PDF)
		
>+ CPU V3.0
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoController_Signal_I2C/IMAGE/CPU3.jpg)
>+ OUTPUT V2.1
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoController_Signal_I2C/IMAGE/OUTPUT2.jpg)
>+ Tower
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoController_Signal_I2C/IMAGE/TOWER_Signal.jpg)
-----------------------------------------------------------------


+ [MattzoController_Switch_I2C](https://github.com/Backkevin/My_LEGO_Project/tree/master/MattzoController_Switch_I2C)

	+ V2.0
		* Work 90%
		* Partlist KO
		* [INO FILE](https://github.com/Backkevin/My_LEGO_Project/tree/master/MattzoController_Switch_I2C/INO/MattzoController_Switch_I2C)
		* [GERBER FILES](https://github.com/Backkevin/My_LEGO_Project/tree/master/MattzoController_Switch_I2C/GERBER%20FILES)
		* [EAGLE FILES](https://github.com/Backkevin/My_LEGO_Project/tree/master/MattzoController_Switch_I2C/EAGLE)
		* [PDF FILES](https://github.com/Backkevin/My_LEGO_Project/tree/master/MattzoController_Switch_I2C/PDF)
		
	+ Work in progress !
>+ CPU V3.0
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoController_Switch_I2C/IMAGE/CPU3.jpg)
>+ PWM V2.0
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoController_Switch_I2C/IMAGE/PWM2.jpg)
-----------------------------------------------------------------


+ [MattzoStationController_I2C](https://github.com/Backkevin/My_LEGO_Project/tree/master/MattzoStationController_I2C)

	+ V2.0
		* Work 90%
		* Partlist KO
		* [INO FILE](https://github.com/Backkevin/My_LEGO_Project/tree/master/MattzoStationController_I2C/INO/MattzoStationController_I2C)
		* [GERBER FILES](https://github.com/Backkevin/My_LEGO_Project/tree/master/MattzoStationController_I2C/GERBER%20FILES)
		* [EAGLE FILES](https://github.com/Backkevin/My_LEGO_Project/tree/master/MattzoStationController_I2C/EAGLE)
		* [PDF FILES](https://github.com/Backkevin/My_LEGO_Project/tree/master/MattzoStationController_I2C/PDF)
		
	+ Work in progress !
>+ CPU V3.0
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoStationController_I2C/IMAGE/CPU3.jpg)
>+ INPUT V2.1
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoController_Sensor_Digital_I2C/IMAGE/INPUT2.jpg)
>+ OUTPUT V2.1
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoController_Signal_I2C/IMAGE/OUTPUT2.jpg)
>+ PWM V2.0
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoController_Switch_I2C/IMAGE/PWM2.jpg)
>+ Tower
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MattzoStationController_I2C/IMAGE/TOWER_Crossing.jpg)
-----------------------------------------------------------------






-----------------------------------------------------------------

+ [MCP23017 Boards I16](https://github.com/Backkevin/My_LEGO_Project/tree/master/MCP23017%20Boards/I16)

	+ V1.0
		* Work 50%
		* Partlist KO
		* [GERBER FILES](https://github.com/Backkevin/My_LEGO_Project/tree/master/MCP23017%20Boards/I16/GERBER%20FILES)
		* [EAGLE FILES](https://github.com/Backkevin/My_LEGO_Project/tree/master/MCP23017%20Boards/I16/EAGLE)
		* [PDF FILES](https://github.com/Backkevin/My_LEGO_Project/tree/master/MCP23017%20Boards/I16/PDF)
		
	+ Work in progress !
>+ CPU V3.0
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MCP23017%20Boards/I16/IMAGE/CPU3.jpg)
>+ I16 V1.0
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MCP23017%20Boards/I16/IMAGE/I16.jpg)
-----------------------------------------------------------------

+ [MCP23017 Boards O8](https://github.com/Backkevin/My_LEGO_Project/tree/master/MCP23017%20Boards/O8)

+ !!!!!!!  max I for the MCP23017 125ma  so we need to change the R470 resistors to 1K2 
+ or
+ dont install the control LEDs on the board so you can keep the R470 resistors

	+ V1.0
		* Work 50%
		* Partlist KO
		* [GERBER FILES](https://github.com/Backkevin/My_LEGO_Project/tree/master/MCP23017%20Boards/O8/GERBER%20FILES)
		* [EAGLE FILES](https://github.com/Backkevin/My_LEGO_Project/tree/master/MCP23017%20Boards/O8/EAGLE)
		* [PDF FILES](https://github.com/Backkevin/My_LEGO_Project/tree/master/MCP23017%20Boards/O8/PDF)
		
	+ Work in progress !
>+ CPU V3.0
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MCP23017%20Boards/O8/IMAGE/CPU3.jpg)
>+ O8 V1.0
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MCP23017%20Boards/O8/IMAGE/O8.jpg)
-----------------------------------------------------------------

+ [MCP23017 Boards I8O4](https://github.com/Backkevin/My_LEGO_Project/tree/master/MCP23017%20Boards/I8O4)

	+ V1.0
		* Work 50%
		* Partlist KO
		* [GERBER FILES](https://github.com/Backkevin/My_LEGO_Project/tree/master/MCP23017%20Boards/I8O4/GERBER%20FILES)
		* [EAGLE FILES](https://github.com/Backkevin/My_LEGO_Project/tree/master/MCP23017%20Boards/I8O4/EAGLE)
		* [PDF FILES](https://github.com/Backkevin/My_LEGO_Project/tree/master/MCP23017%20Boards/I8O4/PDF)
		
	+ Work in progress !
>+ V3.0
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MCP23017%20Boards/I8O4/IMAGE/CPU3.jpg)
>+ V1.0
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/MCP23017%20Boards/I8O4/IMAGE/I8O4.jpg)
-----------------------------------------------------------------






-----------------------------------------------------------------

+ [L9110 Board](https://github.com/Backkevin/My_LEGO_Project/tree/master/L9110%20Board)

	+ V1.0
		* Work 25%
		* Partlist KO
		* [GERBER FILES](https://github.com/Backkevin/My_LEGO_Project/tree/master/L9110%20Board/GERBER%20FILES)
		* [EAGLE FILES](https://github.com/Backkevin/My_LEGO_Project/tree/master/L9110%20Board/EAGLE)
		* [PDF FILES](https://github.com/Backkevin/My_LEGO_Project/tree/master/L9110%20Board/PDF)
		
	+ Work in progress !
>+ CPU V3.0
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/L9110%20Board/IMAGE/CPU3.jpg)
>+ L9110 V1.0
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/L9110%20Board/IMAGE/Default.jpg)
-----------------------------------------------------------------

SENSOR BOARDS
 
-----------------------------------------------------------------
Experimental
-----------------------------------------------------------------

+ Sensor_GP1A57HRJ00F_board_V_1_0
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/Sensor%20Board/IMAGE/Default.jpg)
-----------------------------------------------------------------
Sensor_TCRT5000_board_V_1_0
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/Sensor%20Board/IMAGE/Default.jpg)
-----------------------------------------------------------------
+ Sensor_board_V_1_0
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/Sensor%20Board/IMAGE/Default.jpg)
-----------------------------------------------------------------
+ Sensor_board_2_V_1_0
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/Sensor%20Board/IMAGE/Default.jpg)
-----------------------------------------------------------------
+ Sensor_laser_board_V_1_0
>![](https://github.com/Backkevin/My_LEGO_Project/blob/master/Sensor%20Board/IMAGE/Default.jpg)
-----------------------------------------------------------------

-----------------------------------------------------------------
-----------------------------------------------------------------
>i work on a new software based on MattzoController Firmware V0.41

+ [FULL BRIDGE](https://github.com/Backkevin/My_LEGO_Project/tree/master/Universal%20Driver/FULL%20BRIDGE)     
	* --> for 2 x L9110 Board + 2 x I8O4 Board  
	* --> 4 motors, 8 signals, 24 inputs
	* --> AUTO MODE
	* --> later           
+ [FULL CROSSING](https://github.com/Backkevin/My_LEGO_Project/tree/master/Universal%20Driver/FULL%20CROSSING)  
	* --> for PCA9685 Board + 2 x I8O4 Board + 2 x O8 Board                             
	* --> 32 PWMs, 24 signals, 16 inputs 
	* --> AUTO MODE
	* --> later
+ [FULL ELEVATOR](https://github.com/Backkevin/My_LEGO_Project/tree/master/Universal%20Driver/FULL%20ELEVATOR)
	* --> for 2 x L9110 Board + 2 x I8O4 Board + O8 Board + PCA9685 Board      
	* --> 4 motors, 12 signals, 24 inputs, 32 PWMs
	* --> AUTO MODE
	* --> later
+ [FULL SWITCH](https://github.com/Backkevin/My_LEGO_Project/tree/master/Universal%20Driver/FULL%20SWITCH)   
	* --> for PCA9685 Board + 2 x I8O4 Board + 2 x O8 Board                   
	* --> 32 PWMs, 24 signals, 16 inputs 
	* --> AUTO MODE
	* --> later


+ [ONLY INPUT](https://github.com/Backkevin/My_LEGO_Project/tree/master/Universal%20Driver/ONLY%20INPUT)     
	* --> written for PCF8574 or/and MCP23017                       
	* --> you can use both chips at same time (64+128 inputs) 
	* --> but not tested
+ [ONLY SIGNALS](https://github.com/Backkevin/My_LEGO_Project/tree/master/Universal%20Driver/ONLY%20SIGNAL)   
	* --> written for PCF8574 or/and MCP23017           
	* --> you can use both chips at same time (64+128 outputs) 
	* --> but not tested
+ [ONLY SWITCH](https://github.com/Backkevin/My_LEGO_Project/tree/master/Universal%20Driver/ONLY%20SWITCH)    
	* --> written for PCA9685
	* --> but not tested
+ [ONLY BOOM](https://github.com/Backkevin/My_LEGO_Project/tree/master/Universal%20Driver/ONLY%20BOOM)
	* --> written for PCA9685
	* --> but not tested
