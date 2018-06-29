#include "Arduino.h"
#include "WhaleRTC.h"
#include "RTClib.h"
#include <EEPROM.h>

RTC_PCF8523 rtc;

#define bedTimeMinuteAddress 0
#define bedTimeHourAddress 1
#define wakeupTimeMinuteAddress 2
#define wakeupTimeHourAddress 3

WhaleRTC::WhaleRTC(){}

void WhaleRTC::init(){
  if (! rtc.begin()) {
    Serial.println("RTC Fail. I keep going anyway...");
    Serial.flush();
  }  
  if (! rtc.initialized()) {
    Serial.println("RTC has not been initialized and thus is NOT running!");
    // following line sets the RTC to the date & time from the computer the sketch comes from
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
	rtc.adjust(DateTime(2018, 6, 28, 0, 0, 0));
    Serial.flush();
  }
}

void WhaleRTC::setCurrTime(byte h, byte m)
{
	rtc.adjust(DateTime(2018, 6, 28, h, m, 0));
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

void WhaleRTC::setWakeupTimeMinute(byte val){
  EEPROM.write(wakeupTimeMinuteAddress, val);
}

void WhaleRTC::setWakeupTimeHour(byte val){
  EEPROM.write(wakeupTimeHourAddress, val);
}

byte WhaleRTC::getWakeupTimeMinute(){
  return EEPROM.read(wakeupTimeMinuteAddress);
}

byte WhaleRTC::getWakeupTimeHour(){
  return EEPROM.read(wakeupTimeHourAddress);
}