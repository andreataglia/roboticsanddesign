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
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include "RTClib.h"

Adafruit_8x8matrix leftEye = Adafruit_8x8matrix();
Adafruit_8x8matrix rightEye = Adafruit_8x8matrix();

TMRpcm audioPlayer;

RTC_PCF8523 rtc;

bool presenceDetected = false;

void hardwareSetup()
{  
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  
  pinMode(LED1_R, OUTPUT);
  pinMode(LED1_G, OUTPUT);
  pinMode(LED1_B, OUTPUT);
  pinMode(LED2_R, OUTPUT);
  pinMode(LED2_G, OUTPUT);
  pinMode(LED2_B, OUTPUT);
  pinMode(LED3_R, OUTPUT);
  pinMode(LED3_G, OUTPUT);
  pinMode(LED3_B, OUTPUT);

  //AUDIO
  audioPlayer.speakerPin = SPEAKER_PIN; 
  audioPlayer.setVolume(7);
  audioPlayer.quality(0);

  //LED MATRIX
  leftEye.begin(0x71);  // pass in the address
  rightEye.begin(0x70);  // pass in the address

  leftEye.setBrightness(15);
  leftEye.setRotation(1);
  rightEye.setBrightness(15);
  rightEye.setRotation(1);

  //BLUETOOTH
  Serial3.begin(9600);

  //SD
  if (!SD.begin(SD_CS_PIN)) { 
    Serial.println("SD fail. I keep going anyway...");  
    //return;
  }

  //RTC
  
  if (! rtc.begin()) {
    Serial.println("RTC Fail. I keep going anyway...");
  }  
  if (! rtc.initialized()) {
    Serial.println("RTC is NOT running!");
  }
  Serial.println("Started"); 

  attachInterrupt(digitalPinToInterrupt(PIR_PIN), pir_handler, CHANGE);
  
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

void pir_handler()
{
  if(digitalRead(PIR_PIN)) presenceDetected = true;
  else presenceDetected = false;
}

void makeBitmap(uint8_t rightEyeBMP[], uint8_t leftEyeBMP[])
{
  leftEye.clear();
  //(x, y, bitmap, width, height, color)
  leftEye.drawBitmap(0, 0, leftEyeBMP, 8, 8, LED_ON);
  leftEye.writeDisplay();
  
  rightEye.clear();
  //(x, y, bitmap, width, height, color)
  rightEye.drawBitmap(0, 0, rightEyeBMP, 8, 8, LED_ON);
  rightEye.writeDisplay();  
}

void setup() {
  Serial.begin(9600);
  hardwareSetup();
}

void loop() {
}
