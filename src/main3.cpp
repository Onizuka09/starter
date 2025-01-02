#include <Arduino.h>
#include "network_stack.h"

NetworkStack Network;
// myserv: (192,168,1,14) , 5000
// bilel: (203,161,60,31) , 3500
const char *ssid = "TT_23C0";        //"TP-Link_AP_70CA";
const char *password = "i6q4k3b21r"; //"97800903"; 
void setup(){
Serial.begin(115200);
Network.setServerIp((192,168,1,14) )  ;
Network.setSercerPort(5000);   
Network.configureOTA(true); 
Network.configureWiFi(ssid,password);
Network.InitModule(); 
// Network.

}

void loop(){ 


}