## Modbus and IIoT
![Alt text](https://github.com/iiotntust/1121modbus/blob/main/Modbus_IIoT/New%20Bitmap%20Image.jpg)
### Software required
* Arduino source: https://www.arduino.cc/
* Arduino libraries: esp8266_Modbus, Losant, software serial port
* The Modbus Slave software from "https://github.com/ClassicDIY/ModbusTool"
* A Losant Account (can be made for free: https://www.losant.com/)

### Hands-ON Instruction: 

#### 1. Install the software and connect the device
1. Download and install the software.
2. Download the arduino file and open it on your computers

#### 2. Configure your IIoT platform
1. Sign into your Losant accounts and add a standalone device
2. Add the Attributes "Modbus_Slave_1" to the device
3. Add an Access Key to your Losent profiles.
4. Add a dashboard to show the Attribute as a timegraph
5. Change the Access Tokens and ID Hash based on your own Losant accounts in the Arduino srcipt.
6. upload the arduino file to the D1 board

#### 3. Configure your Modbus Slave
 1. Plug-in the USB/RS485 converter (install the USB/RS485 converter driver, CH340 if necessary)
 2. Open up the Modbus Slave program and change the "Start Address" to "30001"
 3. change the Slave ID to 1
 4. Select RTU mode
 5. Run the slave using the "Listen" button in the menu bar.

## RS485 - Arduino
- RS485 to TTL :https://forum.arduino.cc/t/how-to-read-rs485-data-in-arduino/673285

  
   
