#ifndef WhaleFins_h
#define WhaleFins_h

#include "Arduino.h"
#include <Servo.h>

class WhaleFins
{
  public:
    WhaleFins();
    void init(int);
    void setEmotion(short);
    void stopFins();
  private:
    int _pin;
};

#endif
