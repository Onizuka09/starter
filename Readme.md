# AutoAdStation

## Description:
**AutoAdStation**: is a smart automation system designed for publicity stations. The system ensures seamless startup  of a marketing stand by automating the following processes:
- TV Control via IR: Automatically powers on the TV to begin displaying content without user intervention (Remote control).
- PC Wake-Up via Ethernet (Wake-on-LAN): Wakes the connected PC to run the publicity application as soon as power is restored.

## Features 
- first version of the Starter ( uses OLD IR module, that works only with pulse distance protocole and stores directly the raw data buffer)
- WOL 
- Reads and sends the IR command 
- NO OTA 

### Lib depends 
```
|-- Ethernet @ 2.0.2
|-- SPI @ 2.0.0
|-- ArduinoJson @ 6.20.1
|-- IRremote @ 4.4.1
|-- LittleFS @ 2.0.0
|-- FS @ 2.0.0
```