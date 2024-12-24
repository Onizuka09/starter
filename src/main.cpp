/*
 * Software Verision 0.1 , Moktar SELLAMI
 * @screen_flex
 */

#include "IR_UTILS.h"
#include "RGB_LED.h"
#include "button.h"
#include "mystorage.h"
#include "program_defines.h"
#include "wakeOnLan.h"
#include <Arduino.h>

void setup() {

  Serial.begin(115200);
 
   setupReceiver();
   setupSender();
  // setup task wake PC: perform WOL



  // resetFile(settings) ;
  
}
void loop() {
  // Test_IR_Receive();
   test_receive_and_send_IR();
  
  // delay(1000);
  // IR_RECEIVE_COMMAND();
  // bool btn_state = !digitalRead(PIN_BTN); // Assuming active LOW button
  //  Serial.println(btn_state);

}
