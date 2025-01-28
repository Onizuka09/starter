#ifndef RTC_MOD
#define RTC_MOD
//  #include "RtcDateTime.h"
#include <RtcDS1302.h>
#include <ThreeWire.h>
#include <Arduino.h>
typedef struct
{
  uint8_t day;
  uint8_t month;
  uint8_t year;
  uint8_t hour;
  uint8_t min;
  uint8_t secs;
} dateTime;

class RTC_Module
{
private:
  RtcDateTime start_time;
  uint32_t duration_seconds;
  bool time_func;

  RtcDateTime beginTime;
  RtcDateTime endTime;

  uint8_t _datapin, _clkpin, _rstpin;
  ThreeWire wire;
  RtcDS1302<ThreeWire> Rtc;

public:
  int start_min;
  int prev_min;
  bool timer = false;
  RTC_Module(uint8_t dpin, uint8_t clkpin, uint8_t rstpin);
  void checkOperatingPeriod();
  void RTC_Module_init();
  void printDateTime(RtcDateTime &dt);

  RtcDateTime RTC_getDateTime();

  bool isWithinOperatingPeriod(RtcDateTime now);
  void set_beginTime(RtcDateTime s);
  void set_endTime(RtcDateTime s);
  // TODO:
  // create a func that converts dateTime struct type to RtcDateTime type ;
};

#endif
