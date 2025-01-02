#include <Arduino.h> 
#include <Update.h>

void setup(){ 
Serial.begin(115200); 
Serial.println("HEllo "); 
  bool state= Update.canRollBack(); 
  Serial.println(state == true ?"We can rollback ":  "cannot rollback ");   
  bool x = Update.rollBack();
  Serial.println(x== true ? "ROLLBACK enabled ": "Rollback not enabled");
  delay(1000);
  Serial.println("GIING SOUTH "); 
}
int y = 0; 
void loop(){ 
Serial.println("SOFT VERSION 2 "); 
delay(5000); 
Serial.println("Devision by 0 "); 
Serial.println(5/y); 
}