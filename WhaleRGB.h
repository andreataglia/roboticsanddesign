#ifndef WhaleRGB_h
#define WhaleRGB_h

#include "Arduino.h"

class WhaleRGB
{
  public:
    WhaleRGB(int, int, int);
    void setEmotion(byte);
    void dash();
  private:
    void calcDifference();
    int _pin;
    
};

#endif
