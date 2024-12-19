#ifndef BUTTON_H 
#define BUTTON_H 
#include<Arduino.h>
#define PIN_BTN  12
// 21 




void init_btn(); 
void handleButton_5s() ;
bool isLongPressed_5s();
void TaskResetESP32(void* prameter);
#endif 