## Modbus and IIoT

### Software required
* Arduino source: https://www.arduino.cc/
* Arduino libraries: esp8266_Modbus, Losant, software serial port
* The Modbus Slave software from the "Modbus folder"
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
 2. Open up the Modbus Slave program and change the slave definitions to "input register" and two addresses
 3. change the Slave ID to 1
 4. Run the slave using the connect tap in the menu bar.
  
   
