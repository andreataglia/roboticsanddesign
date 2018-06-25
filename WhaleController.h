#ifndef WhaleController_h
#define WhaleController_h

#include "Arduino.h"
#include "WhaleRGB.h"
#include "WhaleEyes.h"
#include "WhaleFins.h"
#include "WhaleSound.h"
#include "WhaleRTC.h"
#include "WhaleFSM.h"

#define PIR_PIN           19
#define PUSHBUTTON_PIN    18
#define LDR_PIN           A0
#define SERVO_PIN         46

#define LED1_R            13
// #define LED1_G         44
// #define LED1_B         45

#define LED2_R            8
#define LED2_G            9
#define LED2_B            10

#define LED3_R            4
#define LED3_1_G          6
#define LED3_1_B          7


class WhaleController
{
  public:
    WhaleController();
    void init();
    void buttonHandler();
    void pirHandler();
    void setEmotion(short, unsigned long int);
    void initButton(int);
    void initPir(int);
    void initLdr(int);
    void routine();
  private:
    void stopEmotions();
    int btn_pin;
    int pir_pin;
    int ldr_pin;
    int pirState;
    unsigned long int stopEmotionsTime;
    WhaleEyes whaleEyes;
    WhaleRGB whaleRGB;
    WhaleFins whaleFins;
    WhaleSound whaleSound;
    WhaleRTC whaleRTC;
    WhaleFSM whaleFSM;
};

#endif
