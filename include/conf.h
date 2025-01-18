#ifndef MYCONF_H
#define MYCONF_H
#include<Arduino.h>
#include <IPAddress.h>
#include "soft_version.h"
const uint8_t targetMAC[] = {0x00, 0xe0, 0x4c, 0x38, 0xcd, 0xc2}; // BLACK


#define ENABLE_OTA 1
#define DISABLE_ETHERNET 0
#define ENABLE_WIFI 0
#define MyServ      0
#define OTA_WIFI_ETH 0 // 1 WIFI , O ETHERNET  
#define WIFI_PASSWD "i6q4k3b21r" //"97800903";
#define WIFI_SSID  "TT_23C0"        //"TP-Link_AP_70CA";
#define TEST_SOFTWARE_VERSION FIRMWARE_VERSION

#define MYSERV_IP (192, 168, 1, 14);
#define MYSERV_PORT 5000
#define SCREE_FLEX_SERV_IP (203, 161, 60, 31);
#define SCREE_FLEX_SERV_PORT 3500

#if (MyServ == 1)
#define SERVER_IP MYSERV_IP
#define SERVER_PORT MYSERV_PORT
#else 
#define SERVER_IP SCREE_FLEX_SERV_IP
#define SERVER_PORT SCREE_FLEX_SERV_PORT
#endif 
#endif