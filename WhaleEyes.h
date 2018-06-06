#ifndef WhaleEyes_h
#define WhaleEyes_h

#include "Arduino.h"

class WhaleEyes
{
  public:
    WhaleEyes();
    void setEmotion(char);
    void writeMatrix(uint8_t[], uint8_t[]);
};

#endif
