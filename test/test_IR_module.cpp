#include <Arduino.h>
#include "IR_Module.h"
IRHandler irHandler(IR_RECEIVE_PIN, IR_SEND_PIN);

void setup() {
    irHandler.begin();
    pinMode(12,INPUT);
}

void loop() {
    irHandler.receiveIR();
    if (digitalRead(12) != LOW) {
        irHandler.sendIR(); // Send the last stored IR signal when button is pressed
    }
    delay(100);
}