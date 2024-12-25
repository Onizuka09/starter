/*

#ifndef MY_IR_MODULE_H_
#define MY_IR_MODULE_H_
#include <Arduino.h>
#include <IRremote.hpp>
#include <IRProtocol.h>
#include "IR_PIN_DEF.h" 
#include <LittleFS.h>
#include <ArduinoJson.h>
#define f "/settings.json"

 // Define macros for input and output pin etc.
// #include "IR_PIN_DEF.h"

typedef struct {
    bool data_stored; 
    decode_type_t protocol;                 // Protocol type
  IRRawDataType DecodedRawDataArray[RAW_DATA_ARRAY_SIZE] ;
    DistanceWidthTimingInfoStruct timingInfo;  // Used for pulse distance timing
    uint8_t numberOfBits;
              // Number of bits in the received command
    // uint8_t rawData[RAW_BUFFER_LENGTH];  // Raw IR data for Pulse Distance
 
    uint16_t IR_Adress; 
    uint16_t IR_Command;

} MyIRData;


class IRHandler {
public:
    void init_littleFS(); 
    void dump_data_stored(const char* filename);
    IRHandler(uint8_t receivePin, uint8_t sendPin);
    void begin();
    void receiveIR();
    void sendIR();
    decode_type_t identifyProtocol(MyIRData& irData);
    void storeData(MyIRData& irData);
    void playBackData(const MyIRData& irData);
    bool readMyIRDataJSON(const char* filename, MyIRData& data);
    void storeMyIRDataJSON(const char* filename, const MyIRData& data);

private:
    uint8_t receivePin;
    uint8_t _sendPin;
    MyIRData receivedData; // Store the last received IR data
};

void IRHandler::dump_data_stored(const char* filename){ 

    File file = LittleFS.open(filename, "w");
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }
        if ( !file.available()){ 
        Serial.println(" no available content Content:");

        }
        Serial.println("Content:");
        while (file.available()) {
            Serial.write(file.read());
            Serial.flush();
        }
        Serial.println("\n---------------------");
    file.close();
}
void IRHandler::init_littleFS(){ 

     if (!LittleFS.begin()) {
        Serial.println("Failed to initialize LittleFS");
        return ;
    }
}
IRHandler::IRHandler(uint8_t receivePin, uint8_t _sendPin)
    : receivePin(receivePin), _sendPin(_sendPin) {
    // Initialize pins
    // pinMode(receivePin, INPUT);
    // pinMode(sendPin, OUTPUT);
}

// Setup: Begin IRReceiver and IRSender
void IRHandler::begin() {
    Serial.begin(115200);
    IrReceiver.begin(receivePin, ENABLE_LED_FEEDBACK);  // Initialize receiver
    IrSender.begin(_sendPin);  // Initialize sender
    Serial.println("IR Receiver and Sender Initialized");
}

// Receive IR signal and identify protocol
void IRHandler::receiveIR() {
    // IrReceiver.start();
  
    if (IrReceiver.decode()) {
      IrReceiver.printIRResultShort(&Serial); 
      Serial.println(); 
      IrReceiver.printIRResultRawFormatted(&Serial); 
      Serial.println(); 
        MyIRData irData;
        irData.numberOfBits = IrReceiver.decodedIRData.numberOfBits;
        irData.protocol = identifyProtocol(irData);
        // if ( )
        // Store the raw data based on the protocol
        if (irData.protocol == PULSE_DISTANCE) {
            memcpy(irData.DecodedRawDataArray, IrReceiver.decodedIRData.decodedRawDataArray, sizeof(irData.DecodedRawDataArray));
            irData.timingInfo = IrReceiver.decodedIRData.DistanceWidthTimingInfo;
            // irData.data_Read= true; 
        } else {
            irData.IR_Adress = IrReceiver.decodedIRData.address; 
            irData.IR_Command = IrReceiver.decodedIRData.command;
            // irData.data_Read= true; 
            // Handle Samsung or LG protocol
            // Add specific code to handle Samsung and LG data formats
        }

        storeData(irData);
        IrReceiver.resume(); // Ready to receive next data
    }
}

// Identify the protocol type of the received IR signal
decode_type_t IRHandler::identifyProtocol(MyIRData& irData) {
    // Here you can compare against known protocols (e.g., Pulse Distance, Samsung, LG)
    // Serial.println(IrReceiver.decodedIRData.protocol)
    if (IrReceiver.decodedIRData.protocol == PULSE_DISTANCE) {
        return PULSE_DISTANCE;
    } else if (IrReceiver.decodedIRData.protocol == SAMSUNG) {
        return SAMSUNG;
    } else if (IrReceiver.decodedIRData.protocol == LG) {
        return LG;
    }else if (IrReceiver.decodedIRData.protocol == NEC) {
        return NEC;
    } else {
        return UNKNOWN;
    }
}

// Store received IR data in memory
void IRHandler::storeData(MyIRData& irData) {
    // Store IR data in receivedData (you can expand this to use a more advanced storage mechanism)
    receivedData = irData;
    Serial.println("Stored new IR data.");
    receivedData.data_stored=true; 
    storeMyIRDataJSON(f,receivedData);

}

// Send the stored IR data based on protocol
void IRHandler::sendIR() {
        IrReceiver.stop();
        Serial.println("Seding data "); 


    if (receivedData.protocol == PULSE_DISTANCE) {
        Serial.println("SENDING PULSE DISTANCE PROTOCOLE ");
        // IrSender.sendPulseDistanceWidthFromArray(38, &sDistanceWidthTimingInfo, &sDecodedRawDataArray[0], sNumberOfBits,PROTOCOL_IS_LSB_FIRST, 100, 0);
        IrSender.sendPulseDistanceWidthFromArray(38, &receivedData.timingInfo, &receivedData.DecodedRawDataArray[0], receivedData.numberOfBits, PROTOCOL_IS_LSB_FIRST,100,0);
    }else if (receivedData.protocol == NEC ){ 
        IrSender.sendNEC(receivedData.IR_Adress,receivedData.IR_Command,38);
    } 
    else if (receivedData.protocol == SAMSUNG) {
        Serial.println("SENDING SAMSUNG PROTOCOLE ");
        IrSender.sendSamsung(receivedData.IR_Adress,receivedData.IR_Command,38);
        // IrSender.sendRaw(IrReceiver.decodedIRData.rawDataPtr->rawbuf,IrReceiver.decodedIRData.rawlen,38);
        // IrSender.write(IrReceiver.decodedIRData);
        // Send Samsung IR command (implement logic for Samsung)
    } else if (receivedData.protocol == LG) {
        Serial.println("SENDING PULSE LG ");
        IrSender.sendLG(receivedData.IR_Adress,receivedData.IR_Command,38);

        // Send LG IR command (implement logic for LG)
    } else {
        Serial.println("Unknown protocol, cannot send, Protocole not supported .");
        // Serial.printf()
        IrReceiver.printIRResultShort(&Serial);

    }
    IrReceiver.start();
}

// Play back stored IR data (e.g., when button is pressed)
void IRHandler::playBackData(const MyIRData& irData) {
    sendIR();
}

void IRHandler::storeMyIRDataJSON(const char* filename, const MyIRData& data) {
    // if (!LittleFS.begin()) {
    //     Serial.println("Failed to initialize LittleFS");
    //     return;
    // }

    File file = LittleFS.open(filename, "w");
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }

    // Create a JSON object
    // data.data_stored = true; 
    StaticJsonDocument<512> doc;
    doc["stored"]=     data.data_stored ;
    doc["protocol"] = (int)data.protocol;
    doc["numberOfBits"] = data.numberOfBits;
    doc["IR_Adress"] = data.IR_Adress;
    doc["IR_Command"] = data.IR_Command;

    // Serialize the JSON object
    if (serializeJson(doc, file) == 0) {
        Serial.println("Failed to write JSON data");
    } else {
        Serial.println("JSON data stored successfully.");
    }

    file.close();
}

// Read JSON data and deserialize it to MyIRData
bool IRHandler::readMyIRDataJSON(const char* filename, MyIRData& data) {


    File file = LittleFS.open(filename, "r");
    if (!file) {
        Serial.println("Failed to open file for reading");
        return false;
    }

    // Deserialize the JSON object
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, file);
    if (error) {
        Serial.print("Failed to parse JSON data: ");
        Serial.println(error.c_str());
        file.close();
        return false;
    }

    // Populate the structure
    data.protocol = (decode_type_t)doc["protocol"];
    data.numberOfBits = doc["numberOfBits"];
    data.IR_Adress = doc["IR_Adress"];
    data.IR_Command = doc["IR_Command"];

    file.close();
    Serial.println("JSON data read successfully.");
    return true;
}
#endif 
*/