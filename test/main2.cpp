#include <Arduino.h>
// #include <IRremote.h>
// #include "IR_PIN_DEF.h" // Define macros for input and output pin etc.

// Define your IR pin here (adjust as needed)
// #define IR_RECEIVE_PIN 16
// #define IR_SEND_PIN    3
#define SEND_BUTTON_PIN   12
// Maximum IR data size
#define RAW_BUFFER_LENGTH 360

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

// Constructor: Initialize pins for IR receiver and sender
IRHandler::IRHandler(uint8_t receivePin, uint8_t sendPin)
    : receivePin(receivePin), sendPin(sendPin) {
    // Initialize pins
    pinMode(receivePin, INPUT);
    pinMode(sendPin, OUTPUT);
}

// Setup: Begin IRReceiver and IRSender
void IRHandler::begin() {
    Serial.begin(115200);
    IrReceiver.begin(receivePin, ENABLE_LED_FEEDBACK);  // Initialize receiver
    IrSender.begin(sendPin);  // Initialize sender
    Serial.println("IR Receiver and Sender Initialized");
}

// Receive IR signal and identify protocol
void IRHandler::receiveIR() {
    if (IrReceiver.decode()) {
        MyIRData irData;
        irData.numberOfBits = IrReceiver.decodedIRData.numberOfBits;
        irData.protocol = identifyProtocol(irData);

        // Store the raw data based on the protocol
        if (irData.protocol == PULSE_DISTANCE) {
            memcpy(irData.DecodedRawDataArray, IrReceiver.decodedIRData.decodedRawDataArray, sizeof(irData.DecodedRawDataArray));
            irData.timingInfo = IrReceiver.decodedIRData.DistanceWidthTimingInfo;
        } else {
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
}

// Send the stored IR data based on protocol
void IRHandler::sendIR() {
        IrReceiver.stop();
    if (receivedData.protocol == PULSE_DISTANCE) {
        Serial.println("SENDING PULSE DISTANCE PROTOCOLE ");
        // IrSender.sendPulseDistanceWidthFromArray(38, &sDistanceWidthTimingInfo, &sDecodedRawDataArray[0], sNumberOfBits,PROTOCOL_IS_LSB_FIRST, 100, 0);
        IrSender.sendPulseDistanceWidthFromArray(38, &receivedData.timingInfo, &receivedData.DecodedRawDataArray[0], receivedData.numberOfBits, PROTOCOL_IS_LSB_FIRST,100,0);
    } else if (receivedData.protocol == SAMSUNG) {
        Serial.println("SENDING SAMSUNG PROTOCOLE ");

        // Send Samsung IR command (implement logic for Samsung)
    } else if (receivedData.protocol == LG) {
        Serial.println("SENDING PULSE LG ");

        // Send LG IR command (implement logic for LG)
    } else {
        Serial.println("Unknown protocol, cannot send.");
    }
    IrReceiver.start();
}

// Play back stored IR data (e.g., when button is pressed)
void IRHandler::playBackData(const MyIRData& irData) {
    sendIR();
}


IRHandler irHandler(IR_RECEIVE_PIN, IR_SEND_PIN);

void setup() {
    irHandler.begin();
    pinMode(SEND_BUTTON_PIN,INPUT);
}

void loop() {
    irHandler.receiveIR();
    if (digitalRead(SEND_BUTTON_PIN) != LOW) {
        irHandler.sendIR(); // Send the last stored IR signal when button is pressed
    }
    delay(100);
}