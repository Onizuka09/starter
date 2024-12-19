/*
* Software Verision 0.1 , Moktar SELLAMI 
* @screen_flex 
*/

#include <Arduino.h>
#include "program_defines.h"
#include "RGB_LED.h"
#include "button.h"
#include "IR_UTILS.h"
#include "wakeOnLan.h"
#include "mystorage.h"
volatile uint8_t Blinking_status  = 3; // 0: Red, 1: Green, 2: Blue

// pin defs  
uint16_t rawData[] = {
    3950, 4050,
    500, 2000, 500, 2000, 500, 2000, 500, 2000,
    500, 1000, 500, 1000, 500, 2000, 500, 1000,
    500, 2000, 500, 1000, 500, 2000, 500, 1000,
    500, 1000, 500, 1000, 450, 1050, 450, 1050,
    450, 2050, 450, 2050, 450, 1050, 450, 2050,
    450, 1050, 450, 2050, 450, 1050, 450, 2000,
    500
};

void toggleLEDTask(void *parameter) {
  // Set up the RGB pins as outputs
  while (true) {
    
    // Turn on the LED based on the currentColor variable
    switch (Blinking_status) {
      case 0:// green 
           setColor(0,255,0);
           vTaskDelay(500 / portTICK_PERIOD_MS);
           setColor(0,0,0);
vTaskDelay(500 / portTICK_PERIOD_MS);
            break;   
      case 1: 
            setColor(0,0,255);
           vTaskDelay(500 / portTICK_PERIOD_MS);
           setColor(0,0,0);
           vTaskDelay(500 / portTICK_PERIOD_MS); 
           break; // Blue
      case 2:
                 setColor(255,0,0);
           vTaskDelay(500 / portTICK_PERIOD_MS);
           setColor(0,0,0);
           vTaskDelay(500 / portTICK_PERIOD_MS);
            break;  // Red
        case 3:
                 setColor(0,0,0);
           vTaskDelay(500 / portTICK_PERIOD_MS);
           setColor(0,0,0);
           vTaskDelay(500 / portTICK_PERIOD_MS);
            break;  // No color
        case 4:
        setColor(255, 255, 0); // Yellow
        vTaskDelay(500 / portTICK_PERIOD_MS);
        setColor(0, 0, 0);     // Off
        vTaskDelay(500 / portTICK_PERIOD_MS);
        break;

        case 5:
        setColor(173, 216, 230); // Light Blue
        vTaskDelay(500 / portTICK_PERIOD_MS);
        setColor(0, 0, 0);       // Off
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }

    vTaskDelay(500 / portTICK_PERIOD_MS); // Delay for 500ms
  }
}
unsigned long startTime = 0; // Variable to track the start time
bool isScanning = false;     // Variable to indicate if scanning is in progress

void setup(){

    Serial.begin(115200);
    Serial.println("HEllo ...");
    init_btn();
    init_RGB();
    Blinking_status=1; 
    setupReceiver();
    setupSender();
    xTaskCreate(
    toggleLEDTask,       // Function to run as a thread
    "Toggle LED Task",   // Name of the task
    1024,                // Stack size in words
    NULL,                // Task input parameter
    1,                   // Task priority
    NULL                 // Task handle (optional)
  );
  delay(1000);
    Serial.println("HEllo ...");
    Blinking_status=4; 
     init_wol(); 
    init_littlefs();
    Blinking_status=1; 

    // resetFile(settings) ;
    delay(1000);
    Serial.println("Beginning now  ...");
    
    //  
    if ( isDataAvailable(settings)){ 
    Blinking_status=5; 

    dump_file(settings);
    // setColor(0,255,0);
    delay(2000);
    uint8_t temp[255]={0};
    size_t size =255; 
    readData(temp,&size,settings);
    IR_SEND_COMMAND(temp,size); 
    // 
    Serial.println("Waking yp PC "); 
    wakePC();
    // Blinking_status=5; 

    }else { 
    Serial.println("No data is avalaible LITTLEFS IS EMPTY, SCANNING NOW ...."); 
    Blinking_status=2 ; 
        // read IR data 
    // IR_RECEIVE_COMMAND();
    //     if (!isScanning) {
    //   Serial.println("No data is available. LITTLEFS IS EMPTY, SCANNING NOW...");
    //   Blinking_status = 2;
    //   startTime = millis(); // Record the current time
    //   isScanning = true;
    // }

    // Perform the scanning operation
    IR_RECEIVE_COMMAND(true);
    Blinking_status=5; 
  uint8_t temp[255]={0};
    size_t size =255; 
    readData(temp,&size,settings);
    IR_SEND_COMMAND(temp,size); 
    Serial.println("Waking yp PC "); 
    wakePC();
    // Check if 1 minute has elapsed
    // if (millis() - startTime > 60000) { // 1 minute = 60000 milliseconds
    //   Serial.println("1 minute elapsed. Proceeding...");
    //   isScanning = false; // Reset the scanning status
    //   // Add code here to handle the timeout case if needed
    // }
  }
  Blinking_status=0 ; 
    }





void loop() {
    // IR_RECEIVE_COMMAND();
    // bool btn_state = !digitalRead(PIN_BTN); // Assuming active LOW button
    //  Serial.println(btn_state); 

    handleButton(); 
    if ( isLongPressed()){ 
  Blinking_status=3 ; 

  setColor(0,  255, 0); // Green Color
   setColor(255, 0, 0); // Red Color
   delay(1000);
   setColor(0,  255, 0); // Green Color
   delay(1000);
   setColor(0, 0, 255); // Blue Color
   delay(1000);
   setColor(255, 255, 255); // White Color
   delay(1000);
   setColor(170, 0, 255); // Purple Color
   delay(1000);
   setColor(127, 127,  127); // Light Blue
   delay(1000);
    resetFile(settings) ;
    // reset the ESP32 .... 
    delay(1000);
    Serial.println("Resarting now ... ");
    ESP.restart(); 


    }
  Blinking_status=0 ; 
        // Serial.println("yo ;.. ");
    // setColor(0,  0, 255); // Green Color
    // // yield();
    // delay(1000);



    // if (program_status == RESET_IR_COMMAND){ 
    //     Reset_TV_COMMAND();
    //     program_status=INIT;
    // }
}
 

