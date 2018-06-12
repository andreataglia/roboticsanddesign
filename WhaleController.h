#ifndef WhaleController_h
#define WhaleController_h

#include "Arduino.h"
#include "WhaleRGB.h"
#include "WhaleEyes.h"
#include "WhaleFins.h"
#include "WhaleSound.h"


class WhaleController
{
  public:
    WhaleController();
    void init(WhaleRGB, WhaleSound, WhaleFins, WhaleEyes);
    void buttonHandler();
    void pirHandler();
    void setEmotion(short, unsigned long int);
  private:
    
    void initButton(int);
    void initPir(int);
    void initLdr(int);
    void routine();
    
    WhaleFins whaleFins;
    WhaleRGB whaleRGB;
    WhaleEyes whaleEyes;
    WhaleSound whaleSound;
    int btn_pin;
    int pir_pin;
    int ldr_pin;
};

#endif
