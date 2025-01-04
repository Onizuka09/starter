#ifndef NETWORK_STACK_H
#define NETWORK_STACK_H
#include <Arduino.h>
#include <LittleFS.h>
#include <Update.h>
#include <Ethernet.h>
#include <HTTPClient.h>
#include "conf.h"
// MACROS FOR TESTING COMPILATION 

// macors conditions 
/*
NO_HARDWAR: SKIP ethernet module checking 
ENABLE WIFI: ENABLES WIFI MODULE /TODO
MyServ: HTTP connection connetcs to the testing server
*/

class NetworkStack
{

public:

    NetworkStack();

    // initializes both the Ethernet and wifi module 
    void setServerIp(IPAddress ip){serverIP=ip ; }
    void setSercerPort(int port ){ServerPort=port;}
    void configureWiFi(const char *ssid , const char *passwd ); 
    void configureOTA(bool Enable);// this if true will enable OTA and Rollback  
    // void InitModule();
    void InitWifiModule(); 
    void InitEthernetModule(uint8_t cs_pin); 
    // OTA SHIT
    void enableOTA(bool state);
    bool OTA();
    // void OTAPool();// TODO 

    // SERVER COMS  SHIT 
    void NotifyServer();
    const String GetFirmwareVersion();

    
    
    // WOL SHIT 
    void WakeOnLan(const uint8_t TagrgetMac[], int repetition);   
    void initTaskWol();
    void setWoLRepetiton(uint8_t n){WoLRepetion=n;}
    // SETINGS SHIT 
    IPAddress GetServerIP(){return serverIP;};
    uint8_t *GetMacaddr();

private:

    static void runTaskWOL(void* parameter);    
    // void TaskOTAPool();
    String ReadHTTPResponse();
    String HTTPPOST(const String &reqPath, const String &contentType, const String &data);
    String HTTPGET(const String &reqPath);
    int RequestOTA(const String &reqPath); 
    int UpdateFirmware(int FirmwareSize);
    void MacArrayToString(const uint8_t* arr,char* macString,size_t _sizeMacString); 
private:
    TaskHandle_t handler;
    int ServerPort;
    IPAddress serverIP;
    IPAddress LocalIP; // (203, 161, 60, 31); // Example: IP for example.com (or use a domain name)
    uint8_t MyMacAddr[6]; // = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
    bool isOTAEnabled;
    uint8_t WoLRepetion; 

#ifdef USE_ETH
    EthernetClient client;
#else
    WiFiClient client;
    const char *ssid = "";
    const char *passwd = "";
#endif
};

#endif
