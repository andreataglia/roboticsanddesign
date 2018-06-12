#ifndef WhaleRTC_h
#define WhaleRTC_h

#include "Arduino.h"

class WhaleRTC
{
  public:
    WhaleRTC();
    void init();
    void getTime();
};

#endif
