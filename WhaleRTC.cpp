#include "Arduino.h"
#include "WhaleRTC.h"
#include "RTClib.h"
#include <EEPROM.h>

RTC_PCF8523 rtc;

#define bedTimeMinuteAddress 0
#define bedTimeHourAddress 1

WhaleRTC::WhaleRTC(){}

void WhaleRTC::init(){
  if (! rtc.begin()) {
    Serial.println("RTC Fail. I keep going anyway...");
    Serial.flush();
  }  
  if (! rtc.initialized()) {
    Serial.println("RTC has not been initialized and thus is NOT running!");
    // following line sets the RTC to the date & time from the computer the sketch comes from
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Serial.flush();
  }
}

byte WhaleRTC::getCurrHour(){
    return rtc.now().hour();
}

byte WhaleRTC::getCurrMinute(){
    return rtc.now().minute();
}

void WhaleRTC::setBedTimeMinute(byte val){
  EEPROM.write(bedTimeMinuteAddress, val);
}

void WhaleRTC::setBedTimeHour(byte val){
  EEPROM.write(bedTimeHourAddress, val);
}

byte WhaleRTC::getBedTimeMinute(){
  return EEPROM.read(bedTimeMinuteAddress);
}

byte WhaleRTC::getBedTimeHour(){
  return EEPROM.read(bedTimeHourAddress);
}