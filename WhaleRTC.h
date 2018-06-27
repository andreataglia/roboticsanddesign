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
	void setBedTimeHour(byte);
	void setBedTimeMinute(byte);
	byte getBedTimeHour();
	byte getBedTimeMinute();
};

#endif
