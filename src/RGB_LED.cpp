#include "RGB_LED.h"
void init_RGB(){ 
      pinMode(RED,OUTPUT);
    pinMode(GREEN,OUTPUT);
    pinMode(BLUE,OUTPUT);
}
void setColor(int redValue, int greenValue,  int blueValue) {
  analogWrite(RED, redValue);
  analogWrite(GREEN,  greenValue);
  analogWrite(BLUE, blueValue);
}