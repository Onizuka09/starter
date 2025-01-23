#include <Arduino.h>
#include "RTC_module.h"
// #include "RtcDateTime.h"

RTC_Module RTC_mod; 
void setup(){
Serial.begin(115200); 
RTC_mod.RTC_Module_init();

}
void loop(){
     Serial.println("HEllow World form OTA "); 
  RtcDateTime  dt = RTC_mod.RTC_getDateTime();  
     RTC_mod.printDateTime(dt);
// delay(2000);
 /* bool st =  RTC_mod.RTC_SetTimer_min(); */ 

delay(500);
} 


// set up ThreeWire 

