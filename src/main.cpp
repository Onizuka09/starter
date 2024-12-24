// /*
//  * Software Verision 0.1 , Moktar SELLAMI
//  * @screen_flex
//  */


#include <Arduino.h>

#include "IR_PIN_DEF.h" // Define macros for input and output pin etc.

/*
 * Specify which protocol(s) should be used for decoding.
 * If no protocol is defined, all protocols (except Bang&Olufsen) are active.
 * This must be done before the #include <IRremote.hpp>
 */
//#define DECODE_DENON        // Includes Sharp
//#define DECODE_JVC
//#define DECODE_KASEIKYO
//#define DECODE_PANASONIC    // alias for DECODE_KASEIKYO
//#define DECODE_LG
#define DECODE_NEC          // Includes Apple and Onkyo
//#define DECODE_SAMSUNG
//#define DECODE_SONY
//#define DECODE_RC5
//#define DECODE_RC6

//#define DECODE_BOSEWAVE
//#define DECODE_LEGO_PF
//#define DECODE_MAGIQUEST
//#define DECODE_WHYNTER
//#define DECODE_FAST
//


#define RAW_BUFFER_LENGTH  750

//#define EXCLUDE_UNIVERSAL_PROTOCOLS // Saves up to 1000 bytes program memory.
//#define EXCLUDE_EXOTIC_PROTOCOLS // saves around 650 bytes program memory if all other protocols are active
//#define NO_LED_FEEDBACK_CODE      // saves 92 bytes program memory
//#define RECORD_GAP_MICROS 12000   // Default is 8000. Activate it for some LG air conditioner protocols
//#define SEND_PWM_BY_TIMER         // Disable carrier PWM generation in software and use (restricted) hardware PWM.
//#define USE_NO_SEND_PWM           // Use no carrier PWM, just simulate an active low receiver signal. Overrides SEND_PWM_BY_TIMER definition

// MARK_EXCESS_MICROS is subtracted from all marks and added to all spaces before decoding,
// to compensate for the signal forming of different IR receiver modules. See also IRremote.hpp line 142.
#define MARK_EXCESS_MICROS    20    // Adapt it to your IR receiver module. 20 is recommended for the cheap VS1838 modules.

//#define DEBUG // Activate this for lots of lovely debug output from the decoders.

#include <IRremote.hpp>

int SEND_BUTTON_PIN = 12;

int DELAY_BETWEEN_REPEAT = 50;

// Storage for the recorded code
struct storedIRDataStruct {
    IRData receivedIRData;
    // extensions for sendRaw
    uint8_t rawCode[RAW_BUFFER_LENGTH]; // The durations if raw
    uint8_t rawCodeLength; // The length of the code
} sStoredIRData;

bool sSendButtonWasActive;

void storeCode();
void sendCode(storedIRDataStruct *aIRDataToSend);

void setup() {
    pinMode(SEND_BUTTON_PIN, INPUT_PULLUP);

    Serial.begin(115200);

    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

    // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
    Serial.print(F("Ready to receive IR signals of protocols: "));
    Serial.println(F("at pin " STR(IR_RECEIVE_PIN)));
    printActiveIRProtocols(&Serial);
    Serial.println();

    IrSender.begin(IR_SEND_PIN); // Start with IR_SEND_PIN -which is defined in PinDefinitionsAndMore.h- as send pin and enable feedback LED at default feedback LED pin
    Serial.print(F("Ready to send IR signals at pin " STR(IR_SEND_PIN) " on press of button at pin "));
    Serial.println(SEND_BUTTON_PIN);
}

void loop() {

    // If button pressed, send the code.
    bool tSendButtonIsActive = (digitalRead(SEND_BUTTON_PIN) != LOW); // Button pin is active LOW

    /*
     * Check for current button state
     */
    if (tSendButtonIsActive) {
        if (!sSendButtonWasActive) {
            Serial.println(F("Stop receiving"));
            IrReceiver.stop();
        }
        /*
         * Button pressed -> send stored data
         */
        Serial.print(F("Button pressed, now sending "));
        if (sSendButtonWasActive == tSendButtonIsActive) {
            Serial.print(F("repeat "));
            sStoredIRData.receivedIRData.flags = IRDATA_FLAGS_IS_REPEAT;
        } else {
            sStoredIRData.receivedIRData.flags = IRDATA_FLAGS_EMPTY;
        }
        Serial.flush(); // To avoid disturbing the software PWM generation by serial output interrupts
        sendCode(&sStoredIRData);
        delay(DELAY_BETWEEN_REPEAT); // Wait a bit between retransmissions

    } else if (sSendButtonWasActive) {
        /*
         * Button is just released -> activate receiving
         */
        // Restart receiver
        Serial.println(F("Button released -> start receiving"));
        IrReceiver.start();

    } else if (IrReceiver.decode()) {
        /*
         * Button is not pressed and data available -> store received data and resume
         */
        storeCode();
        IrReceiver.resume(); // resume receiver
    }

    sSendButtonWasActive = tSendButtonIsActive;
    delay(100);
}

// Stores the code for later playback in sStoredIRData
// Most of this code is just logging
void storeCode() {
    if (IrReceiver.decodedIRData.rawDataPtr->rawlen < 4) {
        Serial.print(F("Ignore data with rawlen="));
        Serial.println(IrReceiver.decodedIRData.rawDataPtr->rawlen);
        return;
    }
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
        Serial.println(F("Ignore repeat"));
        return;
    }
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_AUTO_REPEAT) {
        Serial.println(F("Ignore autorepeat"));
        return;
    }
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_PARITY_FAILED) {
        Serial.println(F("Ignore parity error"));
        return;
    }
    /*
     * Copy decoded data
     */
    sStoredIRData.receivedIRData = IrReceiver.decodedIRData;

    if (sStoredIRData.receivedIRData.protocol == UNKNOWN) {
        Serial.print(F("Received unknown code and store "));
        Serial.print(IrReceiver.decodedIRData.rawDataPtr->rawlen - 1);
        Serial.println(F(" timing entries as raw "));
        IrReceiver.printIRResultRawFormatted(&Serial, true); // Output the results in RAW format
        sStoredIRData.rawCodeLength = IrReceiver.decodedIRData.rawDataPtr->rawlen - 1;
        /*
         * Store the current raw data in a dedicated array for later usage
         */
        IrReceiver.compensateAndStoreIRResultInArray(sStoredIRData.rawCode);
    } else {
        IrReceiver.printIRResultShort(&Serial);
        IrReceiver.printIRSendUsage(&Serial);
        sStoredIRData.receivedIRData.flags = 0; // clear flags -esp. repeat- for later sending
        Serial.println();
    }
}

void sendCode(storedIRDataStruct *aIRDataToSend) {
  IrReceiver.stop();
    if (aIRDataToSend->receivedIRData.protocol == UNKNOWN /* i.e. raw */) {
        // Assume 38 KHz
        IrSender.sendRaw(aIRDataToSend->rawCode, aIRDataToSend->rawCodeLength, 38);

        Serial.print(F("raw "));
        Serial.print(aIRDataToSend->rawCodeLength);
        Serial.println(F(" marks or spaces"));
    } else {

        /*
         * Use the write function, which does the switch for different protocols
         */
        IrSender.write(&aIRDataToSend->receivedIRData);
        printIRResultShort(&Serial, &aIRDataToSend->receivedIRData, false);
    }
    IrReceiver.start();
}


// // #include "IR_UTILS.h"
// #include "RGB_LED.h"
// #include "button.h"
// #include "mystorage.h"
// #include "program_defines.h"
// #include "wakeOnLan.h"
// #include <Arduino.h>
// volatile uint8_t Blinking_status = 3; // 0: Red, 1: Green, 2: Blue
// volatile bool Wake_PC_STATE = false;
// volatile bool Wake_PC_STATE_finished =false ;
// volatile bool IR_COMMAND_SEND_STATE = false;
// // pin defs
// uint16_t rawData[] = {3950, 4050, 500,  2000, 500,  2000, 500,  2000, 500,
//                       2000, 500,  1000, 500,  1000, 500,  2000, 500,  1000,
//                       500,  2000, 500,  1000, 500,  2000, 500,  1000, 500,
//                       1000, 500,  1000, 450,  1050, 450,  1050, 450,  2050,
//                       450,  2050, 450,  1050, 450,  2050, 450,  1050, 450,
//                       2050, 450,  1050, 450,  2000, 500};
// void WakePCTask(void *parameter) {
//   while ( true){ 
//   if (true == Wake_PC_STATE) {
//     wakePC();
//     Wake_PC_STATE = false;
//     Wake_PC_STATE_finished = true ;
//       vTaskDelete(NULL); 
//   }
//       vTaskDelay(100 / portTICK_PERIOD_MS);
//   }
// }
// unsigned long  btime = 0 ;  

// // void
// void toggleLEDTask(void *parameter) {
//   // Set up the RGB pins as outputs
//   while (true) {

//     // Turn on the LED based on the currentColor variable
//     switch (Blinking_status) {
//     case 0: // green
//       setColor(0, 255, 0);
//       vTaskDelay(500 / portTICK_PERIOD_MS);
//       setColor(0, 0, 0);
//       vTaskDelay(500 / portTICK_PERIOD_MS);
//       break;
//     case 1:
//       setColor(0, 0, 255);
//       vTaskDelay(500 / portTICK_PERIOD_MS);
//       setColor(0, 0, 0);
//       vTaskDelay(500 / portTICK_PERIOD_MS);
//       break; // Blue
//     case 2:
//       setColor(255, 0, 0);
//       vTaskDelay(500 / portTICK_PERIOD_MS);
//       setColor(0, 0, 0);
//       vTaskDelay(500 / portTICK_PERIOD_MS);
//       break; // Red
//     case 3:
//       setColor(0, 0, 0);
//       vTaskDelay(500 / portTICK_PERIOD_MS);
//       setColor(0, 0, 0);
//       vTaskDelay(500 / portTICK_PERIOD_MS);
//       break; // No color
//     case 4:
//       setColor(255, 255, 0); // Yellow
//       vTaskDelay(500 / portTICK_PERIOD_MS);
//       setColor(0, 0, 0); // Off
//       vTaskDelay(500 / portTICK_PERIOD_MS);
//       break;

//     case 5:
//       setColor(173, 216, 230); // Light Blue
//       vTaskDelay(500 / portTICK_PERIOD_MS);
//       setColor(0, 0, 0); // Off
//       vTaskDelay(500 / portTICK_PERIOD_MS);
//       break;
//     default:
//       break ;
//     }

//     // vTaskDelay(500 / portTICK_PERIOD_MS); // Delay for 500ms
//   }
// }

// void setup() {

//   Serial.begin(115200);
//   init_littlefs();
//   delay(500);
//   Blinking_status = 1;
//   xTaskCreate(toggleLEDTask,     // Function to run as a thread
//               "Toggle LED Task", // Name of the task
//               1024,              // Stack size in words
//               NULL,              // Task input parameter
//               3,                 // Task priority
//               NULL               // Task handle (optional)
//   );
//   Serial.println("HEllo ...");
//   init_btn();
//   init_RGB();
//   // setupReceiver();
//   // setupSender();
//   xTaskCreate(WakePCTask, // Function to run as a thread
//               "Wake PC ", // Name of the task
//               1024,       // Stack size in words
//               NULL,       // Task input parameter
//               1,          // Task priority
//               NULL        // Task handle (optional)
//   );

//   xTaskCreate(TaskResetESP32, // Function to run as a thread
//               "Reset ESP ", // Name of the task
//               1024,       // Stack size in words
//               NULL,       // Task input parameter
//               2,          // Task priority
//               NULL        // Task handle (optional)
//   );
//   // setup task wake PC: perform WOL
//   delay(50);
//   Serial.println("HEllo ...");
//   Blinking_status = 4;
//   init_wol();
//   Blinking_status = 1;


//   // resetFile(settings) ;
//   delay(1000);
//   Serial.println("Beginning now  ...");

//   // Wake_PC_STATE = true;
//   // // Wake_PC_STATE_Running = true ; 
//   // if (isDataAvailable(settings)) {
//   //   Blinking_status = 5;

//   //   dump_file(settings);
//   //   // setColor(0,255,0);
//   //   delay(50);
//   //   uint8_t temp[255] = {0};
//   //   size_t size = 255;
//   //   readData(temp, &size, settings);
//   //   IR_SEND_COMMAND(temp, size);
//   //   //
//   //   // Serial.println("Waking yp PC ");
//   //   // wakePC();
//   //   // Blinking_status=5;

//   // } else {
//   //   Serial.println("No data is avalaible LITTLEFS IS EMPTY, SCANNING NOW ....");
//   //   Blinking_status = 2;

//   //   // Perform the scanning operation
//   //   IR_RECEIVE_COMMAND(true);
//   //   Blinking_status = 5;
//   //   uint8_t temp[255] = {0};
//   //   size_t size = 255;
//   //   readData(temp, &size, settings);
//   //   IR_SEND_COMMAND(temp, size);
//   //   Serial.println("Waking yp PC ");
//   //   // wakePC();
//   //   // Check if 1 minute has elapsed
//   //   // if (millis() - startTime > 60000) { // 1 minute = 60000 milliseconds
//   //   //   Serial.println("1 minute elapsed. Proceeding...");
//   //   //   isScanning = false; // Reset the scanning status
//   //   //   // Add code here to handle the timeout case if needed
//   //   // }
//   // }

//   Blinking_status = 0;
//   btime=millis();
// }
// void loop() {
//   // IR_RECEIVE_COMMAND();
//   // bool btn_state = !digitalRead(PIN_BTN); // Assuming active LOW button
//   //  Serial.println(btn_state);

//   handleButton_5s();
//   if (isLongPressed_5s()) {
//     Blinking_status = 3;

//     // setColor(0, 255, 0); // Green Color
//     // setColor(255, 0, 0); // Red Color
//     // delay(1000);
//     // setColor(0, 255, 0); // Green Color
//     // delay(1000);
//     // setColor(0, 0, 255); // Blue Color
//     // delay(1000);
//     // setColor(255, 255, 255); // White Color
//     // delay(1000);
//     // setColor(170, 0, 255); // Purple Color
//     // delay(1000);
//     // setColor(127, 127, 127); // Light Blue
//     // delay(100  0);
//     resetFile(settings);

//     Blinking_status = 2;
//     // reset the ESP32 ....
//     delay(50);
//     Serial.println("IR Code has been RESETR ... ");
//     //  IR_RECEIVE_COMMAND(true);
//     uint8_t temp[255] = {0};
//     size_t size = 255;
//     readData(temp, &size, settings);
//     btime = millis(); 
    
//     // IR_SEND_COMMAND(temp, size);
//     // Serial.println("Waking yp PC ");
//     // ESP.restart();
//   }
//   if ( millis() - btime <= 3000) { 
//     Blinking_status = 0;
  
//     }else { 
//       Blinking_status=3 ; 
//   }

//   // Serial.println("yo ;.. ");
//   // setColor(0,  0, 255); // Green Color
//   // // yield();
//   // delay(1000);

//   // if (program_status == RESET_IR_COMMAND){
//   //     Reset_TV_COMMAND();
//   //     program_status=INIT;
//   // }
// }
