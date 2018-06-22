#ifndef WhaleRTC_h
#define WhaleRTC_h

#include "Arduino.h"

class WhaleRTC
{
  public:
    WhaleRTC();
    void init();
    byte getCurrMinute();
    byte getCurrHour();
};

#endif
