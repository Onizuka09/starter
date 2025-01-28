#include "RTC_module.h"
#include "HardwareSerial.h"
#include "RtcDateTime.h"

#define DAT 21
#define CLK 22
#define RST 14
ThreeWire wire(DAT, CLK, RST);
RtcDS1302<ThreeWire> Rtc(wire);

void RTC_Module::RTC_Module_init()
{
  Serial.begin(115200);
  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);

  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  printDateTime(compiled);
  Serial.println();

  if (!Rtc.IsDateTimeValid())
  {
    Serial.println("RTC lost confidence in the DateTime!");
    Rtc.SetDateTime(compiled);
  }

  if (Rtc.GetIsWriteProtected())
  {
    Serial.println("RTC was write protected, enabling writing now");
    Rtc.SetIsWriteProtected(false);
  }

  if (!Rtc.GetIsRunning())
  {
    Serial.println("RTC was not actively running, starting now");
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  Serial.println("==================================");
  Serial.println("Current time:");
  printDateTime(now);
  Serial.println("==================================");

  if (now < compiled)
  {
    Serial.println("RTC is older than compile time! (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  }
  else if (now > compiled)
  {
    Serial.println("RTC is newer than compile time. (this is expected)");
  }
  else if (now == compiled)
  {
    Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }
}

void RTC_Module::printDateTime(RtcDateTime &dt)
{
  char datestring[20];
  snprintf_P(datestring, sizeof(datestring), PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
             dt.Month(), dt.Day(), dt.Year(), dt.Hour(), dt.Minute(), dt.Second());
  Serial.println(datestring);
}

RtcDateTime RTC_Module::RTC_getDateTime()
{
  RtcDateTime now = Rtc.GetDateTime();
  if (!now.IsValid())
  {
    Serial.println("Error: Lost confidence in RTC");
  }
  return now;
}

bool RTC_Module::isWithinOperatingPeriod(RtcDateTime currentTime)
{
  return (currentTime >= beginTime && currentTime < endTime);
}

void RTC_Module::checkOperatingPeriod()
{
  RtcDateTime currentTime = RTC_getDateTime();
  Serial.print("Current Time: ");
  printDateTime(currentTime);

  if (isWithinOperatingPeriod(currentTime))
  {
    Serial.println("Message: Operating hours - Program is running.");
  }
  else
  {
    Serial.println("Message: Outside operating hours - Program is idle.");
  }
}