#ifndef WhaleRTC_h
#define WhaleRTC_h

#include "Arduino.h"

class WhaleRTC
{
  public:
    WhaleRTC();
    void init();
	void setCurrTime(byte, byte);
    byte getCurrMinute();
    byte getCurrHour();
	void setBedTimeHour(byte);
	void setBedTimeMinute(byte);
	byte getBedTimeHour();
	byte getBedTimeMinute();
	void setWakeupTimeHour(byte);
	void setWakeupTimeMinute(byte);
	byte getWakeupTimeHour();
	byte getWakeupTimeMinute();
};

#endif
