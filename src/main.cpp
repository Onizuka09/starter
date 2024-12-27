// /*
//  * Software Verision 0.1 , Moktar SELLAMI
//  * @screen_flex
//  */





// #include "IR_UTILS.h"
#include "RGB_LED.h"
#include "button.h"
#include "IR_Module.h"
#include "mystorage.h"
#include "program_defines.h"
#include "wakeOnLan.h"
#include "Mylog.h"
#include <Arduino.h>
typedef enum{ 
GREEN_COLOR_STATUS=0,
BLUE_COLOR_STATUS,
RED_COLOR_STATUS,
NO_COLOR_STATUS,
YELLOW_COLOR_STATUS,
LIGHT_BLUE_COLOR_STATUS,
// GREEN_COLOR_STATUS,
// GREEN_COLOR_STATUS,
// GREEN_COLOR_STATUS,
}LED_STATUS_COLOR;
IRHandler irHandler(IR_RECEIVE_PIN, IR_SEND_PIN);

volatile uint8_t Blinking_status = BLUE_COLOR_STATUS; // 0: Red, 1: Green, 2: Blue
volatile bool Wake_PC_STATE = false;
volatile bool Wake_PC_STATE_finished =false ;
volatile bool IR_COMMAND_SEND_STATE = false;
// pin defs
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
    MyLog(DEBUG,"WAKING PC FINISHED");
  }
      vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
unsigned long  btime = 0 ;  

// void
void toggleLEDTask(void *parameter) {
  // Set up the RGB pins as outputs
    MyLog(INFO,"TOOGLE TASK Started ..."); 

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
void init_tasks(){ 
  // task To toggle LED 
  xTaskCreate(toggleLEDTask,     // Function to run as a thread
              "Toggle LED Task", // Name of the task
              1024,              // Stack size in words
              NULL,              // Task input parameter
              3,                 // Task priority
              NULL               // Task handle (optional)
  );
  // TASK to RESET program when button pressed for more than10s 
  xTaskCreate(TaskResetESP32, // Function to run as a thread
              "Reset ESP ", // Name of the task
              1024,       // Stack size in words
              NULL,       // Task input parameter
              2,          // Task priority
              NULL        // Task handle (optional)
  );
}
void setup() {
  Serial.begin(115200);
  MyLog(INFO,"Booting into STARTER ESP32 Version 2..."); 
  MyLog(INFO,""); 
  MyLog(INFO,""); 


  Blinking_status = (int)BLUE_COLOR_STATUS;  
  // init storege and RGB LED 
  irHandler.begin();
  irHandler.init_littleFS();
  // CLEARs json file  
  // MyLogF(INFO,"Clearing the %s file ",f); 
  // irHandler.clear_file(f);
  // init RGB 
  init_RGB(); 
  // init Button 12 
  init_btn();
  // init tasks  
  init_tasks();
  // init WOL
  Blinking_status = (int)YELLOW_COLOR_STATUS;
  init_wol();
  Blinking_status = (int)BLUE_COLOR_STATUS;
  // task to send WOL PACKETS ( wake PC)
  xTaskCreate(WakePCTask, // Function to run as a thread
              "Wake PC ", // Name of the task
              1024,       // Stack size in words
              NULL,       // Task input parameter
              1,          // Task priority
              NULL        // Task handle (optional)
  );
 
  MyLog(INFO,"Finished Initializing ..."); 
  
  delay(50);

  // setup task wake PC: perform WOL
  
  MyLog(INFO,"Starting Now ..."); 
  MyLog(DEBUG,"STARTING WOL  ..."); 
  Wake_PC_STATE = true;
  MyLog(DEBUG,"STARTING IR  ..."); 


  bool state = irHandler.IsIRDataavailable(f);
  if (state ){//isDataAvailable(settings)) {
    Blinking_status = LIGHT_BLUE_COLOR_STATUS;
    MyLog(DEBUG,"COMMAND FOUND  ..."); 
    // irHandler.dump_data_stored(f); 
    MyIRData data; 
    MyLog(DEBUG,"Reading data from json file ..."); 

    irHandler.DumpMyIRDataJSON(f); 
    irHandler.readMyIRDataJSON(f,data);
    MyLog(DEBUG,"Printing data  ..."); 
    irHandler.PrintMyIRData(data); 
    irHandler.setRecievedData(data); 
    irHandler.sendIR(); 

    // Blinking_status=GR;

  } else {
    MyLog(ERROR,"No data is avalaible LITTLEFS IS EMPTY, SCANNING NOW ....");

    // Perform the scanning operation
    Blinking_status=(int)RED_COLOR_STATUS;
    
    irHandler.receiveIR(true);
    Blinking_status = LIGHT_BLUE_COLOR_STATUS;

    irHandler.sendIR();
    MyLog(INFO,"Finished Sending IR data ..."); 
    // IR_RECEIVE_COMMAND(true);
    // Blinking_status = 5;
    // uint8_t temp[255] = {0};
    // size_t size = 255;
    // readData(temp, &size, settings);
    // IR_SEND_COMMAND(temp, size);
    // Serial.println("Waking yp PC ");
    // wakePC();
    // Check if 1 minute has elapsed
    // if (millis() - startTime > 60000) { // 1 minute = 60000 milliseconds
    //   Serial.println("1 minute elapsed. Proceeding...");
    //   isScanning = false; // Reset the scanning status
    //   // Add code here to handle the timeout case if needed
    // }
  }

  Blinking_status = (int)GREEN_COLOR_STATUS;
  btime=millis();
}
void loop() {
  // IR_RECEIVE_COMMAND();
  // bool btn_state = !digitalRead(PIN_BTN); // Assuming active LOW button
  //  Serial.println(btn_state);

  handleButton_5s();
  if (isLongPressed_5s()) {
     MyLog(ERROR,"BUtton PRESSD for more than 5s ....");
    Blinking_status = (int)RED_COLOR_STATUS;
    irHandler.clear_file(f);


    // Perform the scanning operation
    
    while (!irHandler.receiveIR(true)){ 
      Blinking_status = RED_COLOR_STATUS;

    }
    Blinking_status = (int)GREEN_COLOR_STATUS;
    // RESET FILE 
    // resetFile(settings);
    Blinking_status=(int)RED_COLOR_STATUS;
    
    irHandler.receiveIR(true);
    Blinking_status=(int)GREEN_COLOR_STATUS;
    
    // rRead new IR COMMAND
    // delay(50);
    // Serial.println("IR Code has been RESETR ... ");
    // //  IR_RECEIVE_COMMAND(true);
    // uint8_t temp[255] = {0};
    // size_t size = 255;
    // readData(temp, &size, settings);
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
