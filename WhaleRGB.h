#ifndef WhaleRGB_h
#define WhaleRGB_h

#include "Arduino.h"

class WhaleRGB
{
  public:
    WhaleRGB();
    void init(int, int, int, int, int, int, int, int, int);
    void setEmotion(byte);
  private:
    void calcDifference();
    
};

#endif
