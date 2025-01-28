#ifndef RTC_MOD
#define RTC_MOD
//  #include "RtcDateTime.h"
#include <RtcDS1302.h>
#include <ThreeWire.h>
#include <Arduino.h>

class RTC_Module
{
private:
  RtcDateTime start_time;
  uint32_t duration_seconds;
  bool time_func;

  RtcDateTime beginTime;
  RtcDateTime endTime;

public:
  int start_min;
  int prev_min;
  bool timer = false;
  RTC_Module() : start_min(0), prev_min(0), time_func(false) {}
  void RTC_Timer(uint32_t seconds);
  void checkOperatingPeriod();
  void RTC_Module_init();
  void printDateTime(RtcDateTime &dt);

  RtcDateTime RTC_getDateTime();
  bool isElapsed();
  bool RTC_SetTimer_min();
  bool isWithinOperatingPeriod(RtcDateTime now);
  void set_beginTime(RtcDateTime s)
  {
    beginTime = s;
  }

  void set_endTime(RtcDateTime s)
  {
    endTime = s;
  }
};

#endif
