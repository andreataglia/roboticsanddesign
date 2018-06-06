#define SPEAKER_PIN       5
#define PIR_PIN           19
#define PUSHBUTTON_PIN    18
#define LDR_PIN           A0
#define SD_CS_PIN         49
#define SERVO_PIN         46

#define LED1_R            13
#define LED1_G            44
#define LED1_B            45

#define LED2_R            8
#define LED2_G            9
#define LED2_B            10

#define LED3_R            4
#define LED3_G            6
#define LED3_B            7

#define LDR_TRIGGERVALUE 400

#include <SD.h>
#include <TMRpcm.h>
#include <SPI.h>
#include "RTClib.h"
#include "WhaleRGB.h"
#include "WhaleEyes.h"

TMRpcm audioPlayer;
RTC_PCF8523 rtc;
WhaleEyes whaleEyes;

#define ACTIVATION 0
#define JOY 1
#define FEAR 2
#define ANGER 3
#define SAD 4
#define DISGUST 5
#define NEUTRAL 6

bool presenceDetected = false;

void hardwareSetup()
{  
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);

  //AUDIO
  audioPlayer.speakerPin = SPEAKER_PIN; 
  audioPlayer.setVolume(7);
  audioPlayer.quality(0);

  //BLUETOOTH
  Serial3.begin(9600);

  //SD
  if (!SD.begin(SD_CS_PIN)) { 
    Serial.println("SD fail. I keep going anyway...");  
    Serial.flush();
    //return;
  }

  //RTC
  
  if (! rtc.begin()) {
    Serial.println("RTC Fail. I keep going anyway...");
    Serial.flush();
  }  
  if (! rtc.initialized()) {
    Serial.println("RTC is NOT running!");
    Serial.flush();
  }
  whaleEyes.init();
  Serial.println("Started"); 
  Serial.flush();

  //attachInterrupt(digitalPinToInterrupt(PIR_PIN), pir_handler, CHANGE);
}

void getTime(byte* h, byte* m)
{
    DateTime now = rtc.now();

    *h = now.hour();
    *m = now.minute();
}

bool getState()
{
  if(analogRead(LDR_PIN) > LDR_TRIGGERVALUE) return true;
  else return false;
}

//void pir_handler()
//{
//  if(digitalRead(PIR_PIN)) presenceDetected = true;
//  else presenceDetected = false;
//}


void setup() {
  Serial.begin(9600);
  hardwareSetup();
  WhaleRGB whaleRGB(LED1_R, LED1_G, LED1_B);
  whaleRGB.setEmotion(ANGER);
  whaleEyes.setEmotion(ANGER);
}

void loop() {
  
}
