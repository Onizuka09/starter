#ifndef MYCONF_H
#define MYCONF_H
#include<Arduino.h>
#include <IPAddress.h>
const uint8_t targetMAC[] = {0x00, 0xe0, 0x4c, 0x38, 0xcd, 0xc2}; // BLACK

#define TEST_SOFTWARE_VERSION "1.0.0"

#define ENABLE_OTA 1
#define DISABLE_ETHERNET 1
#define ENABLE_WIFI 1
#define MyServ      0

#define WIFI_PASSWD "i6q4k3b21r" //"97800903";
#define WIFI_SSID  "TT_23C0"        //"TP-Link_AP_70CA";

#define MYSERV_IP (192, 168, 1, 14);
#define MYSERV_PORT 5000
#define SCREE_FLEX_SERV_IP (203, 161, 60, 31);
#define SCREE_FLEX_SERV_PORT 3500
    
#endif