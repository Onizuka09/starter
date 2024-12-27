#include <Arduino.h>
#include "IR_Module.h"
#include "mystorage.h"
IRHandler irHandler(IR_RECEIVE_PIN, IR_SEND_PIN);

void setup() {
    Serial.begin(115200);
    irHandler.begin();
    irHandler.init_littleFS(); 
    
    pinMode(12,INPUT);
    listDir(LittleFS,"/",1); 
    Dump_file(LittleFS, file_names[jsettings].c_str());
    // irHandler.dump_data_stored(f);
}

void loop() {
    irHandler.receiveIR();
    if (digitalRead(12) != LOW) {
        irHandler.sendIR(); // Send the last stored IR signal when button is pressed
        irHandler.DumpMyIRDataJSON(f);

    }
    delay(100);
}