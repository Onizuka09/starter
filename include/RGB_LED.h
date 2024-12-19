#ifndef RGB_LED_H 
#define RGB_LED_H 
#include <Arduino.h>
#define RED 27
#define GREEN 26
#define BLUE 25


void init_RGB();
void setColor(int redValue, int greenValue,  int blueValue); 
#endif 
