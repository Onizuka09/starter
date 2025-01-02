<<<<<<< HEAD
# AutoAdStation

## Description:
**AutoAdStation**: is a smart automation system designed for publicity stations. The system ensures seamless startup  of a marketing stand by automating the following processes:
- TV Control via IR: Automatically powers on the TV to begin displaying content without user intervention (Remote control).
- PC Wake-Up via Ethernet (Wake-on-LAN): Wakes the connected PC to run the publicity application as soon as power is restored.



### Lib depends 
```
|-- Ethernet @ 2.0.2
|-- SPI @ 2.0.0
|-- ArduinoJson @ 6.20.1
|-- IRremote @ 4.4.1
|-- LittleFS @ 2.0.0
|-- FS @ 2.0.0
=======
# PlatformIO Template in NEOVIM

### Build for `ESP32-1`:
```bash
pio run -e ESP32-1
```
- Upload for ESP32-1 (First time using Serial COM11):
```bash
pio run -e ESP32-1 --target upload
```
- Monitor Serial Output for ESP32-1:
```bash
pio device monitor -e ESP32-1
```
- Build for ESP32-2:
```bash
pio run -e ESP32-2
```
- Upload for ESP32-2 (Using OTA IP 192.168.2.240):
bash
Copy code
```bash
pio run -e ESP32-2 --target upload
```
- Monitor Serial Output for ESP32-2:
```bash
pio device monitor -e ESP32-2
```
### Additional Notes:
- Switch Between Serial and OTA Upload:
For the first-time upload, use the serial port (COM11 for ESP32-1 or COM7 for ESP32-2).

After the first upload, switch to OTA by uncommenting the upload_port line with the device IP address in platformio.ini.

- Recompile Both Environments:

If you uncomment the default_envs for both ESP32-1 and ESP32-2, you can build all environments together:

```bash
pio run
```
- Upload to Both Devices:

```bash
pio run --target upload
>>>>>>> OTA
```