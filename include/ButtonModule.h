/**
 * @file ButtonModule.h
 * @brief Button handling module.
 * This module provides functionality to
 * - initialize a button
 * - detect long presses 5s ,
 * - manage tasks related to reset an ESP32 based on a duration of pressed button.
 * @author Moktar SELLAMI
 * @date 2025-02-16
 */
#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

/**
 * @class ButtonModule
 * @brief A class to handle button-related functionalities.
 */

class ButtonModule
{
public:
    /**
     * @brief Constructor to initialize the button module.
     * @param _pinbtn The GPIO pin number where the button is connected.
     */
    ButtonModule(uint8_t _pinbtn);

    /**
     * @brief Initializes the button by setting up the pin mode.
     */
    void InitButton();

    /**
     * @brief Handles button press detection for 5 seconds.
     *
     * This function checks if the button is continuously pressed for 5 seconds
     * and triggers the corresponding action.
     */
    void HandleButton_5s();

    /**
     * @brief Checks if the button is pressed for 5 seconds.
     * @return True if the button has been pressed for at least 5 seconds, otherwise false.
     */
    bool isLongPressed_5s();

    /**
     * @brief Starts a task to reset the ESP32.
     *
     * This function creates a FreeRTOS task that handles the ESP32 reset process, if the button was pressed for certain amount of time.
     */
    void runResetESP32Task();

    /**
     * @brief Stops the ESP32 reset task.
     *
     * This function suspends or stops the reset task if it is running.
     */
    void stopResetESP32Task();

    /**
     * @brief Terminates and removes the ESP32 reset task.
     *
     * This function forcefully deletes the reset task from FreeRTOS.
     */
    void killResetESP32Task();

private:
    /**
     * @brief Task function to reset the ESP32.
     * @param parameter Pointer to task parameters (if any).
     */
    static void TaskResetESP32(void *parameter);

private:
    uint8_t pinbtn;       /**< GPIO pin number where the button is connected. */
    TaskHandle_t handler; /**< FreeRTOS task handle for the reset task. */
};

#endif // BUTTON_H
