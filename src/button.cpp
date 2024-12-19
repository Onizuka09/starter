#include"button.h"

static unsigned long pressStart = 0;
static bool longPressed = false;

void init_btn(){ 
    pinMode(PIN_BTN,INPUT);
}
void handleButton() {
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
        if (millis() - period > 5000 && !longPressed) { // Long press detected
            Serial.println("btn pressed for more than 5s");
            longPressed = true;
            btn_pressed = false; // Reset for new detection
            // program_status = RESET_IR_COMMAND;
            return ; // Long press detected
        }
    } else { // Button is released
        btn_pressed = false; // Reset press state
        longPressed = false; // Allow new long press detection
        period = 0; // Reset the timer
        // program_status = INIT;
    }

    return ; // No long press detected
}


bool isLongPressed() {
    return longPressed;
}
