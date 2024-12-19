#include"button.h"

static unsigned long pressStart = 0;
static bool longPressed_5s = false;
static bool longPressed_10s = false;
extern volatile bool Wake_PC_STATE_finished ;


void init_btn(){ 
    pinMode(PIN_BTN,INPUT);
}
void handleButton_5s() {
    static bool btn_pressed = false;
    // static bool long_pressed_d11etected = false;
    static unsigned long period = 0;
    
    bool btn_state = digitalRead(PIN_BTN);//! // Assuming active LOW button
    /* Serial.println(btn_state); */
    if (btn_state) { // Button is pressed
        if (!btn_pressed) { // First press detected
            btn_pressed = true;
            period = millis(); // Record the time of the press
        }
        if (millis() - period >= 5000 && !longPressed_5s) { // Long press detected
            Serial.println("btn pressed for more than 5s");
            longPressed_5s = true;
            btn_pressed = false; // Reset for new detection
            // program_status = RESET_IR_COMMAND;
            // return ; // Long press detected
        }
        if ( millis() - period >= 10000){ 
            Serial.println("Reseting ESP32 NOW ..... ");
            longPressed_10s = true;
            btn_pressed = false; // Reset for new detection
            
            return ; 
        }
    } else { // Button is released
        btn_pressed = false; // Reset press state
        longPressed_10s = false ;
        longPressed_5s = false; // Allow new long press detection
        period = 0; // Reset the timer
        // program_status = INIT;
    }

    return ; // No long press detected
}

void TaskResetESP32(void* prameter){ 
    static bool btn_pressed = false;
    // static bool long_pressed_d11etected = false;
    static unsigned long period = 0;
    while (1){ 
        if ( Wake_PC_STATE_finished){
            // Serial.println("task running ....");
            bool btn_state = digitalRead(PIN_BTN);//! // Assuming active LOW button
            /* Serial.println(btn_state); */
            if (btn_state) { // Button is pressed
                if (!btn_pressed) { // First press detected
                    btn_pressed = true;
                    period = millis(); // Record the time of the press
                }

                if ( millis() - period >= 10000){ 
                    Serial.println("Reseting ESP32 NOW ..... ");
                    btn_pressed = false; // Reset for new detection

                    ESP.restart();
                }
            } else { // Button is released
                btn_pressed = false; // Reset press state

                period = 0; // Reset the timer
            }
              vTaskDelay(50 / portTICK_PERIOD_MS);

            }
    }
    
}

bool isLongPressed_5s() {
    return longPressed_5s;
}
