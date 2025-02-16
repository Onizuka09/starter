/**
 * @file RTC_module.h
 * @brief This file defines the RTC_Module class for controlling and managing the real-time clock (RTC).
 * 
 * The RTC_Module class provides functionality to initialize the RTC, set timers, and retrieve date-time information.
 * @author Moktar SELLAMI
 * @date 2025-02-16 
 */

 #ifndef RTC_MOD
 #define RTC_MOD
 
 //  #include "RtcDateTime.h"
 #include <RtcDS1302.h>
 #include <ThreeWire.h>
 #include <Arduino.h>
 
 /**
  * @class RTC_Module
  * @brief Class for managing and interacting with the RTC (Real-Time Clock) module.
  * 
  * This class allows setting a timer, initializing the RTC, and retrieving the current date and time from the RTC module.
  */
 class RTC_Module
 {
 private:
     RtcDateTime start_time; /**< Stores the starting date and time for the timer. */
     uint32_t duration_seconds; /**< Duration of the timer in seconds. */
     bool time_func; /**< Flag indicating whether the time function is active. */
 
 public:
     int start_min; /**< The starting minute for the timer. */
     int prev_min; /**< The previous minute value to track changes. */
     bool timer = false; /**< Flag indicating whether the timer is running. */
 
     /**
      * @brief Constructor for the RTC_Module class.
      * Initializes the starting and previous minutes and the time function flag.
      */
     RTC_Module() : start_min(0), prev_min(0), time_func(false) {}
 
     /**
      * @brief Sets the timer for the specified duration in seconds.
      * 
      * @param seconds The duration of the timer in seconds.
      */
     void RTC_Timer(uint32_t seconds);
 
     /**
      * @brief Initializes the RTC module and sets up the required pins.
      */
     void RTC_Module_init();
 
     /**
      * @brief Prints the current date and time from the RTC module.
      * 
      * @param dt The date and time object to be printed.
      */
     void printDateTime(RtcDateTime &dt);
 
     /**
      * @brief Retrieves the current date and time from the RTC.
      * 
      * @return The current date and time as an RtcDateTime object.
      */
     RtcDateTime RTC_getDateTime();
 
     /**
      * @brief Checks if the specified duration has elapsed based on the timer.
      * 
      * @return True if the timer duration has elapsed, false otherwise.
      */
     bool isElapsed();
 
     /**
      * @brief Sets the timer based on the minute value, called periodically.
      * 
      * @return True if the timer was set successfully, false otherwise.
      */
     bool RTC_SetTimer_min();
 };
 
 #endif
 