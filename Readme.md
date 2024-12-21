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
```