// /*
//  * Software Verision 1.2.1 , Moktar SELLAMI
//  * @screen_flex
//  */

// #include "IR_UTILS.h"
#include "conf.h"
#include "soft_version.h"
#include "network_stack.h"
#include "Hardware.h"
#include "RGBModule.h"
#include "ButtonModule.h"
#include "IR_Module.h"
// #include "program_defines.h"
#include "Mylog.h"
#include <Arduino.h>

IRHandler irHandler(IR_RECEIVE_PIN, IR_SEND_PIN_);
ButtonModule btn(PIN_BTN);
RGBModule rgb(RGB_RED_PIN, RGB_GREEN_PIN, RGB_BLUE_PIN);
NetworkStack network;
volatile uint8_t Blinking_status = BLUE_COLOR_STATUS; // 0: Red, 1: Green, 2: Blue
volatile bool Wake_PC_STATE = false;
volatile bool Wake_PC_STATE_finished = false;
// pin defs

unsigned long btime = 0;

void setup()
{
  Serial.begin(115200);
  MyLog(INFO, "----------------------------------------");
  MyLogF(INFO, "Runing %s with version %s", PRODUCT_NAME, FIRMWARE_VERSION);
  MyLog(INFO, "----------------------------------------");

  // init storege and RGB LED
  irHandler.begin();
  irHandler.init_littleFS(); // TODO: add option to clear FS

  // init RGB
  rgb.RGBInit();
  rgb.setBlinkingStatus(BLUE_COLOR_STATUS);
  // init_RGB();
  // init Button 12
  btn.InitButton();
  // init tasks
  rgb.runRGBTask();
  // initize the ethernet Module
  rgb.setBlinkingStatus(YELLOW_COLOR_STATUS);

  network.InitEthernetModule(W5500_CS);
  rgb.setBlinkingStatus(BLUE_COLOR_STATUS);

  MyLog(INFO, "Finished Initializing all the Modules  ...");
  MyLog(INFO, "Starting Now ...");

  delay(50);
  MyLog(DEBUG, "STARTING WOL TASK ...");
  Wake_PC_STATE = false;
  network.initTaskWol();

  MyLog(DEBUG, "STARTING IR  ...");

  bool state = irHandler.IsIRDataavailable(f);
  if (state)
  { // isDataAvailable(settings)) {
    rgb.setBlinkingStatus(LIGHT_BLUE_COLOR_STATUS);
    MyLog(DEBUG, "COMMAND FOUND  ...");
    // irHandler.dump_data_stored(f);
    MyIRData data;
    MyLog(DEBUG, "Reading data from json file ...");
    // irHandler.DumpMyIRDataJSON(f);
    irHandler.readMyIRDataJSON(f, data);
    // MyLog(DEBUG,"Printing data  ...");
    // irHandler.PrintMyIRData(data);
    irHandler.setRecievedData(data);
    irHandler.sendIR();

    // Blinking_status=GR;
  }
  else
  {
    MyLog(ERROR, "No data is avalaible LITTLEFS IS EMPTY, SCANNING NOW ....");

    // Perform the scanning operation
    rgb.setBlinkingStatus(RED_COLOR_STATUS);

    irHandler.receiveIR(true);
    rgb.setBlinkingStatus(LIGHT_BLUE_COLOR_STATUS);

    irHandler.sendIR();
    MyLog(INFO, "Finished Sending IR data ...");
  }

  rgb.setBlinkingStatus(GREEN_COLOR_STATUS);
  btime = millis();
  // OTA handlin ...
#if (ENABLE_OTA == 1)
  IPAddress ip SERVER_IP;
  network.setServerIp(ip);
  network.setSercerPort(SERVER_PORT);
  network.configureWiFi(WIFI_SSID, WIFI_PASSWD);
  network.InitWifiModule();
  MyLog(INFO, "Performing OTA ");
  network.configureOTA(true);

  bool st = network.OTA();
  if (st)
  {
    MyLog(INFO, "RESTATRING OTA ");
    ESP.restart();
  }
  else
  {
    MyLog(INFO, " OTA Failed miserably ");
  }
#else
  MyLog(INFO, " OTA is disabled ");

#endif
}
void loop()
{

  btn.HandleButton_5s();
  if (btn.isLongPressed_5s())
  {
    MyLog(ERROR, "BUtton PRESSD for more than 5s ....");
    rgb.setBlinkingStatus(RED_COLOR_STATUS);
    // clearr the json file that holds the IR data
    irHandler.clear_file(f);

    irHandler.receiveIR(true);
    rgb.setBlinkingStatus(GREEN_COLOR_STATUS);

    btime = millis();

    // OPT restart the ESP
    // ESP.restart();
  }
  if (millis() - btime <= 3000)
  {
    Blinking_status = 0;
    rgb.setBlinkingStatus(GREEN_COLOR_STATUS);
  }
  else
  {
    rgb.setBlinkingStatus(NO_COLOR_STATUS);
  }
}
