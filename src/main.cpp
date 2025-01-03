// /*
//  * Software Verision 0.1 , Moktar SELLAMI
//  * @screen_flex
//  */





// #include "IR_UTILS.h"
#include "soft_version.h"
#include "Hardware.h"
#include "RGBModule.h"
#include "button.h"
#include "IR_Module.h" 
#include "program_defines.h"
#include "wakeOnLan.h"
#include "Mylog.h"
#include <Arduino.h>

IRHandler irHandler(IR_RECEIVE_PIN, IR_SEND_PIN_);

RGBModule rgb(RGB_RED_PIN,RGB_GREEN_PIN,RGB_BLUE_PIN); 
volatile uint8_t Blinking_status = BLUE_COLOR_STATUS; // 0: Red, 1: Green, 2: Blue
volatile bool Wake_PC_STATE = false;
volatile bool Wake_PC_STATE_finished =false ;
volatile bool IR_COMMAND_SEND_STATE = false;
// pin defs

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

void init_tasks(){ 
  // task To toggle LED 

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
  MyLog(INFO,"----------------------------------------");
  MyLogF(INFO,"Runing %s with version %s",PRODUCT_NAME,FIRMWARE_VERSION);
  MyLog(INFO,"----------------------------------------");


  // init storege and RGB LED 
  irHandler.begin();
  irHandler.init_littleFS(); // TODO: add option to clear FS 
 
  // init RGB 
  rgb.RGBInit();
  rgb.setBlinkingStatus(BLUE_COLOR_STATUS);  
  // init_RGB(); 
  // init Button 12 
  init_btn();
  // init tasks  
  rgb.runRGBTask(); 
  // init_tasks();
  // init WOL
  rgb.setBlinkingStatus(YELLOW_COLOR_STATUS);
  // init_wol();
  delay(1000);
  rgb.setBlinkingStatus(BLUE_COLOR_STATUS);
  // task to send WOL PACKETS ( wake PC)
  xTaskCreate(WakePCTask, // Function to run as a thread
              "Wake PC ", // Name of the task
              1024,       // Stack size in words
              NULL,       // Task input parameter
              1,          // Task priority
              NULL        // Task handle (optional)
  );
 
  MyLog(INFO,"Finished Initializing all the Modules  ..."); 
  MyLog(INFO,"Starting Now ..."); 

  delay(50);
  MyLog(DEBUG,"STARTING WOL TASK ..."); 
  Wake_PC_STATE = false;
  MyLog(DEBUG,"STARTING IR  ..."); 


  bool state = irHandler.IsIRDataavailable(f);
  if (state ){//isDataAvailable(settings)) {
    rgb.setBlinkingStatus(LIGHT_BLUE_COLOR_STATUS);
    MyLog(DEBUG,"COMMAND FOUND  ..."); 
    // irHandler.dump_data_stored(f); 
    MyIRData data; 
    MyLog(DEBUG,"Reading data from json file ..."); 
    // irHandler.DumpMyIRDataJSON(f); 
    irHandler.readMyIRDataJSON(f,data);
    // MyLog(DEBUG,"Printing data  ..."); 
    // irHandler.PrintMyIRData(data); 
    irHandler.setRecievedData(data); 
    irHandler.sendIR(); 

    // Blinking_status=GR;

  } else {
    MyLog(ERROR,"No data is avalaible LITTLEFS IS EMPTY, SCANNING NOW ....");

    // Perform the scanning operation
   rgb.setBlinkingStatus(RED_COLOR_STATUS);
    
    irHandler.receiveIR(true);
    rgb.setBlinkingStatus(LIGHT_BLUE_COLOR_STATUS);

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

  rgb.setBlinkingStatus(GREEN_COLOR_STATUS);
  btime=millis();
}
void loop() {
  // IR_RECEIVE_COMMAND();
  // bool btn_state = !digitalRead(PIN_BTN); // Assuming active LOW button
  //  Serial.println(btn_state);

  handleButton_5s();
  if (isLongPressed_5s()) {
    MyLog(ERROR,"BUtton PRESSD for more than 5s ....");
    rgb.setBlinkingStatus(RED_COLOR_STATUS);
    irHandler.clear_file(f);


    // Perform the scanning operation
    
    while (!irHandler.receiveIR(true)){ //TODO: FIX THIS CHANANIGIN
      rgb.setBlinkingStatus(RED_COLOR_STATUS);

    }
    rgb.setBlinkingStatus(GREEN_COLOR_STATUS);
    // RESET FILE 
    // resetFile(settings);
    rgb.setBlinkingStatus(RED_COLOR_STATUS);
    
    irHandler.receiveIR(true);
    rgb.setBlinkingStatus(GREEN_COLOR_STATUS);
    
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
    rgb.setBlinkingStatus(GREEN_COLOR_STATUS);
  
    }else { 
      rgb.setBlinkingStatus(NO_COLOR_STATUS);
  }


}
