// #include "IR_UTILS.h"

#include <IRremote.hpp>
#include "IR_PIN_DEF.h"
#include "mystorage.h"
#define max_buff_size 255 
size_t Buffer_size; 
uint8_t buffer[max_buff_size]={0}; 
static bool processReceivedData();
static void handleDecodedData();
static void handleErrorFlags();
static void sendIRData(uint8_t* buff, size_t len);
static void saveIRCommand(); 
void Test_IR_Receive(){ 
     IrReceiver.start();
    // printActiveIRProtocols(&Serial);
    if (IrReceiver.decode()) {
    printIRResultShort(&Serial,&IrReceiver.decodedIRData,true);
    Serial.println("------------------");
    // Serial.println()
    IrReceiver.printIRResultRawFormatted(&Serial,true);
    Serial.println("DONE ");

    }   
    

    IrReceiver.stop();


}
void test_receive_and_send_IR(){ 
      if (IrReceiver.decode()) {


            printIRResultShort(&Serial,&IrReceiver.decodedIRData,true);
        if (IrReceiver.decodedIRData.decodedRawData) {

            Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
            // We have an unknown protocol here, print extended info
            IrReceiver.printIRResultRawFormatted(&Serial, true);
            // Serial.println(IrReceiver.decodedIRData.decodedRawData )    ;
            Serial.println("-----------------");
            size_t len = IrReceiver.decodedIRData.rawDataPtr->rawlen - 1 ; 
            IRRawbufType *rawd = IrReceiver.decodedIRData.rawDataPtr->rawbuf ; 
            uint8_t buff[len]; 
            int j = 0 ; 
            for (int i =1 ; i <=len ; i ++   ){     
                Serial.print ("  "); 
                Serial.print (rawd[i]*_MICROS_PER_TICK);
                buff[i]=rawd[i]*_MICROS_PER_TICK;  
                Serial.print ("  "); 
                j ++ ; 
                if ( j >=4 ){ 
                    j=0 ; 
                    Serial.println();
                }
            }           
            IrReceiver.resume(); // Do it here, to preserve raw data for printing with printIRResultRawFormatted()
            Serial.println("Sending data ... "); 
            IrReceiver.stop(); 
            delay(500);
            IrSender.sendRaw(IrReceiver.decodedIRData.rawDataPtr->rawbuf,IrReceiver.decodedIRData.rawlen,38);
            delay(500);
            Serial.println("data sent "); 
            IrReceiver.start();



     
        }
    }
}
void IR_RECEIVE_COMMAND(bool store_data) {
    IrReceiver.start();
    while ( true ){ 
    if (IrReceiver.decode()) {
        if (!processReceivedData()) {
            Serial.println(F("Error in received data!"));
        }
        else { 
            break ;
        }

    }   
    }

    IrReceiver.stop();

    if (store_data){ 
       Serial.println("Storing data ...."); 
       delay(1000); 
       write_file(buffer,sizeof(buffer), settings);
       Serial.println("data Stored ... "); 

    }
}
void IR_SEND_COMMAND(uint8_t* buff,size_t s ){ 
    IrReceiver.stop();
    delay(500);
    for ( int i = 0 ; i <51 ; i ++ ){ 
        buff[i]*= _MICROS_PER_TICK ;
    }
    sendIRData(buff, s );
    delay(50);
    Serial.println(F("IR COMMAND Data sent ..."));
    // IrReceiver.start();
}
// Initialize the IR receiver
void setupReceiver() {
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
    Serial.print(F("Ready to receive IR signals of protocols: "));
    printActiveIRProtocols(&Serial);
    Serial.println(F(" at pin " STR(IR_RECEIVE_PIN)));
}

// Initialize the IR sender
void setupSender() {
    IrSender.begin(IR_SEND_PIN);
    Serial.print(F("Ready to send IR signals at pin " STR(_IR_SEND_PIN)));
}

// Process the received IR data
bool processReceivedData() {
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_EMPTY ||
        IrReceiver.decodedIRData.flags & IRDATA_FLAGS_PARITY_FAILED ||
        IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_AUTO_REPEAT ||
        IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
        handleErrorFlags();
        return false;
    }

    if (IrReceiver.decodedIRData.decodedRawData == 0) {
        Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
        IrReceiver.printIRResultRawFormatted(&Serial, true);
        return false;
    }

    handleDecodedData();
    return true;
}

// Handle decoded IR data
void handleDecodedData() {
     Buffer_size = IrReceiver.decodedIRData.rawDataPtr->rawlen - 1;
    IRRawbufType* rawd = IrReceiver.decodedIRData.rawDataPtr->rawbuf;
    // uint8_t buff[Buffer_size];
    int j = 0;

    Serial.println(F("Decoded raw data:"));
    for (int i = 1; i <= Buffer_size; i++) {
        Serial.print("  ");
        Serial.print(rawd[i] * _MICROS_PER_TICK);
        buffer[i - 1] = rawd[i] ;//* _MICROS_PER_TICK;
        Serial.print("  ");
        j++;
        if (j >= 4) {
            j = 0;
            Serial.println();
        }
    }

    Serial.println(F("\nSending data..."));

}

// Handle error flags
void handleErrorFlags() {
    Serial.print(F("Error flags detected: "));
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_EMPTY) {
        Serial.print(F("EMPTY "));
    }
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_PARITY_FAILED) {
        Serial.print(F("PARITY_FAILED "));
    }
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_AUTO_REPEAT) {
        Serial.print(F("AUTO_REPEAT "));
    }
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
        Serial.print(F("REPEAT "));
    }
    Serial.println();
}

// Send the IR data
void sendIRData(uint8_t* buff, size_t len) {
    IrSender.sendRaw(buff, len, 38); // Send the data at 38kHz
}
