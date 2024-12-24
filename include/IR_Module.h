#ifndef IR_MODULE_H_
#define IR_MODULE_H_ 
#include <Arduino.h>
#include "IR_PIN_DEF.h" // Define macros for input and output pin etc.
#include <IRProtocol.h>

// Define your IR pin here (adjust as needed)
// #define IR_RECEIVE_PIN 16
// #define IR_SEND_PIN    3
// #define SEND_BUTTON_PIN   12
// Maximum IR data size
// #define RAW_BUFFER_LENGTH 360

// Define the types of protocols
// enum IRProtocol {
//     PULSE_DISTANCE,
//     SAMSUNG,
//     LG,
//     UNKNOWN
// };

// Structure to hold IR data for Pulse Distance, Samsung, and LG protocols
typedef struct {
    IRRawDataType DecodedRawDataArray[RAW_DATA_ARRAY_SIZE] ;
    // uint8_t rawData[RAW_BUFFER_LENGTH];  // Raw IR data for Pulse Distance
    uint8_t numberOfBits;                // Number of bits in the received command
    decode_type_t protocol;                 // Protocol type
    DistanceWidthTimingInfoStruct timingInfo;  // Used for pulse distance timing
} MyIRData;

class IRHandler {
public:
    IRHandler(uint8_t receivePin, uint8_t sendPin);
    void begin();
    void receiveIR();
    void sendIR();
    decode_type_t identifyProtocol(MyIRData& irData);
    void storeData(MyIRData& irData);
    void playBackData(const MyIRData& irData);

private:
    uint8_t receivePin;
    uint8_t sendPin;
    MyIRData receivedData; // Store the last received IR data
};

#endif 