#include "Arduino.h"
#include "WhaleRTC.h"

#include "RTClib.h"

RTC_PCF8523 rtc;

WhaleRTC::WhaleRTC(){}

void WhaleRTC::init(){
  if (! rtc.begin()) {
    Serial.println("RTC Fail. I keep going anyway...");
    Serial.flush();
  }  
  if (! rtc.initialized()) {
    Serial.println("RTC is NOT running!");
    Serial.flush();
  }
}

void WhaleRTC::getTime(byte* h, byte* m){
    DateTime now = rtc.now();
    *h = now.hour();
    *m = now.minute();
}
