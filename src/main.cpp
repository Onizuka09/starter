

#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <Ethernet.h>
#define OTETHERNET
#include <ArduinoOTA.h>
// #include "credentials.h"
const char* ssid = "TP-Link_AP_70CA";
const char* password = "97800903";

unsigned long previoustime=0;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // Replace with your W5500 MAC address

void setup() {
    Serial.begin(115200);
      Ethernet.init(33);
    Ethernet.begin(mac);
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
        Serial.println("Ethernet shield not found!");
        while (true);
    }

    if (Ethernet.linkStatus() == LinkOFF) {
        Serial.println("Ethernet cable not connected!");
    }

    Serial.println("Ethernet connected");
    delay(100);
    Serial.println("Booting ESP32-1- OTA");
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(Ethernet.localIP());
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, password);    // make sure to enter your wifi credentials on "credential.h"
//   while (WiFi.waitForConnectResult() != WL_CONNECTED) {
//     Serial.println("Connection Failed! Rebooting...");
//     delay(500);
//     // ESP.restart();
//   }
    
  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
//    ArduinoOTA.setHostname("ESP32-1");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  
  ArduinoOTA.begin();


}

void loop() {
  ArduinoOTA.handle();

 long currenttime= millis();
  if (currenttime-previoustime >=5000) {  //Print only every 5seconds
    previoustime=currenttime;
    Serial.print("This is ESP32-1 updated , ");   
    Serial.print("IP address: ");
    Serial.println(Ethernet.localIP());
  
  }
 
}