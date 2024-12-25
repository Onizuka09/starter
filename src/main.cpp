// /*
//  * Software Verision 0.1 , Moktar SELLAMI
//  * @screen_flex
//  */


#include <Arduino.h>
#include "soft_version.h"
#include "IR_UTILS.h"
#include "RGB_LED.h"
#include "button.h"
#include "mystorage.h"
#include "program_defines.h"
#include "wakeOnLan.h"
#include <Arduino.h>
volatile uint8_t Blinking_status = 3; // 0: Red, 1: Green, 2: Blue
volatile bool Wake_PC_STATE = false;
volatile bool Wake_PC_STATE_finished =false ;
volatile bool IR_COMMAND_SEND_STATE = false;
// example of raw data 
uint16_t rawData[] = {3950, 4050, 500,  2000, 500,  2000, 500,  2000, 500,
                      2000, 500,  1000, 500,  1000, 500,  2000, 500,  1000,
                      500,  2000, 500,  1000, 500,  2000, 500,  1000, 500,
                      1000, 500,  1000, 450,  1050, 450,  1050, 450,  2050,
                      450,  2050, 450,  1050, 450,  2050, 450,  1050, 450,
                      2050, 450,  1050, 450,  2000, 500};
void WakePCTask(void *parameter) {
  while ( true){ 
  if (true == Wake_PC_STATE) {
    wakePC();
    Wake_PC_STATE = false;
    Wake_PC_STATE_finished = true ;
      vTaskDelete(NULL); 
  }
      vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
unsigned long  btime = 0 ;  

// void
void toggleLEDTask(void *parameter) {
  // Set up the RGB pins as outputs
  while (true) {

    // Turn on the LED based on the currentColor variable
    switch (Blinking_status) {
    case 0: // green
      setColor(0, 255, 0);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      setColor(0, 0, 0);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      break;
    case 1:
      setColor(0, 0, 255);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      setColor(0, 0, 0);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      break; // Blue
    case 2:
      setColor(255, 0, 0);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      setColor(0, 0, 0);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      break; // Red
    case 3:
      setColor(0, 0, 0);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      setColor(0, 0, 0);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      break; // No color
    case 4:
      setColor(255, 255, 0); // Yellow
      vTaskDelay(500 / portTICK_PERIOD_MS);
      setColor(0, 0, 0); // Off
      vTaskDelay(500 / portTICK_PERIOD_MS);
      break;

    case 5:
      setColor(173, 216, 230); // Light Blue
      vTaskDelay(500 / portTICK_PERIOD_MS);
      setColor(0, 0, 0); // Off
      vTaskDelay(500 / portTICK_PERIOD_MS);
      break;
    default:
      break ;
    }

    // vTaskDelay(500 / portTICK_PERIOD_MS); // Delay for 500ms
  }
}

void setup() {

  Serial.begin(115200);

  Serial.println("----------------------------------------");
  Serial.printf("Runing %s with version %.1lf\n\r",PRODUCT_NAME,VERSION);
  Serial.println("----------------------------------------");


  init_littlefs();
  delay(500);
  Blinking_status = 1;
  xTaskCreate(toggleLEDTask,     // Function to run as a thread
              "Toggle LED Task", // Name of the task
              1024,              // Stack size in words
              NULL,              // Task input parameter
              3,                 // Task priority
              NULL               // Task handle (optional)
  );
  Serial.println("HEllo ...");
  init_btn();
  init_RGB();
  setupReceiver();
  setupSender();
  xTaskCreate(WakePCTask, // Function to run as a thread
              "Wake PC ", // Name of the task
              1024,       // Stack size in words
              NULL,       // Task input parameter
              1,          // Task priority
              NULL        // Task handle (optional)
  );

  xTaskCreate(TaskResetESP32, // Function to run as a thread
              "Reset ESP ", // Name of the task
              1024,       // Stack size in words
              NULL,       // Task input parameter
              2,          // Task priority
              NULL        // Task handle (optional)
  );
  // setup task wake PC: perform WOL
  delay(50);
  Serial.println("HEllo ...");
  Blinking_status = 4;
  init_wol();
  Blinking_status = 1; 
  delay(1000);
  Serial.println("Beginning now  ...");

  Wake_PC_STATE = true;
  // Wake_PC_STATE_Running = true ; 
  if (isDataAvailable(settings)) {
    Blinking_status = 5;

    dump_file(settings);
    delay(50);
    uint8_t temp[255] = {0};
    size_t size = 255;
    readData(temp, &size, settings);
    IR_SEND_COMMAND(temp, size);
    //
    Serial.println("Waking yp PC ");
    wakePC();
    Blinking_status=5;

  } else {
    Serial.println("No data is avalaible LITTLEFS IS EMPTY, SCANNING NOW ....");
    Blinking_status = 2;

    // Perform the scanning operation
    IR_RECEIVE_COMMAND(true);
    Blinking_status = 5;
    uint8_t temp[255] = {0};
    size_t size = 255;
    readData(temp, &size, settings);
    IR_SEND_COMMAND(temp, size);
    Serial.println("Waking yp PC ");
    wakePC();
    // Check if 1 minute has elapsed
    // if (millis() - startTime > 60000) { // 1 minute = 60000 milliseconds
    //   Serial.println("1 minute elapsed. Proceeding...");
    //   isScanning = false; // Reset the scanning status
    //   // Add code here to handle the timeout case if needed
    // }
  }
  Blinking_status = 0;
  btime=millis();
}
void loop() {


  handleButton_5s();
  if (isLongPressed_5s()) {
    Blinking_status = 3;

    resetFile(settings);

    Blinking_status = 2;
    // reset the ESP32 ....
    delay(50);
    Serial.println("IR Code has been RESETR ... ");
    //  IR_RECEIVE_COMMAND(true);
    uint8_t temp[255] = {0};
    size_t size = 255;
    readData(temp, &size, settings);
    btime = millis(); 
    
    // IR_SEND_COMMAND(temp, size);
    // Serial.println("Waking yp PC ");
    // ESP.restart();
  }
  if ( millis() - btime <= 3000) { 
    Blinking_status = 0;
  
    }else { 
      Blinking_status=3 ; 
  }

}
