### Modbus: (Software)
* Modbus Tools source: https://www.modbustools.com/ (only 30-day trials)
* ModbusPoll: install on PC to read the devices in the network via RS485 or LAN.
* ModbusSlave: install on PC to simulate a device with Modbus Protocols (Modbus/RTU, TCP)
* Modbustool.7z: executable file (.exe), unlimited time to use, but Chinese GUI only
* USB/RS485 converter driver CH340 (source:https://www.wch.cn/download/CH341SER_EXE.html )
### Hands-ON Instruction: 
#### 1. Install the software and connect the device
1. download Modbus tools and Install them (including USB/RS485 converter driver, CH340)
2. Plug-in
3. 
#### 2. PC-to-PC
1. Configure ModbusSlave on PC#1 for creating registers data
2. Use PC#2 with ModbusPoll to read the data from PC#1
#### 3. PC-to-PowerMeter
1. Use read power meter voltage and current by PC with ModbusPoll
