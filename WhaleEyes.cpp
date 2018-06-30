#include "Arduino.h"
#include "WhaleEyes.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#define EXPRESSIONS 7

Adafruit_8x8matrix leftEye = Adafruit_8x8matrix();
Adafruit_8x8matrix rightEye = Adafruit_8x8matrix();

uint8_t
  offLeft_bpm[]=
  { B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000},
  offRight_bpm[]=
  { B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000},
//JOY
  joyLeft_bpm[]=
  { B00000000,
    B00000000,
    B00111000,
    B01000100,
    B01000100,
    B01000100,
    B00000000,
    B00000000 },
  joyRight_bpm[]=
  { B00000000,
    B00011100,
    B00100010,
    B00100010,
    B00100010,
    B00000000,
    B00000000,
    B00000000 },
//FEAR
  fearLeft_bmp[] =
  { B00000000,
    B00000000,
    B00000000,
    B11111100,
    B00001000,
    B00010000,
    B00100000,
    B00000000 },
  fearRight_bmp[] =
  { B00000000,
    B00000000,
    B00111111,
    B00010000,
    B00001000,
    B00000100,
    B00000000,
    B00000000 },
//ANGER
  angerLeft_bpm[] =
  { B00000000,
    B00000000,
    B01100000,
    B01010000,
    B01001100,
    B01000010,
    B01111111,
    B00000000 },
  angerRight_bpm[] =
  { B00000000,
    B00000110,
    B00001010,
    B00110010,
    B01000010,
    B11111110,
    B00000000,
    B00000000 },
//SAD
  sadLeft_bpm[] =
  { B00000000,
    B00000000,
    B00000100,
    B00000100,
    B00001100,
    B00011000,
    B11100000,
    B00000000 },
  sadRight_bpm[] =
  { B00000000,
    B00100000,
    B00100000,
    B00110000,
    B00011000,
    B00000111,
    B00000000,
    B00000000 },
//DISGUST
  disgustLeft_bmp[] =
  { B00000000,
    B00000000,
    B01111110,
    B00011000,
    B00011000,
    B00011000,
    B00011000,
    B00000000 },
  disgustRight_bmp[] =
  { B00000000,
    B01111110,
    B00011000,
    B00011000,
    B00011000,
    B00011000,
    B00000000,
    B00000000 },
//NEUTRAL
  neutralLeft_bpm[] = 
  { B00000000,
    B00011100,
    B00110000,
    B01100000,
    B01001100,
    B00011100,
    B00011000,
    B00000000 },
  neutralRight_bpm[] = 
  { B00111000,
    B00001100,
    B00000110,
    B00110010,
    B00111000,
    B00011000,
    B00000000,
    B00000000 };

uint8_t* bpmIdx[EXPRESSIONS*2] = {
  &offLeft_bpm[0], &offRight_bpm[0],
  &joyLeft_bpm[0], &joyRight_bpm[0],
  &fearLeft_bmp[0], &fearRight_bmp[0],
  &angerLeft_bpm[0], &angerRight_bpm[0],
  &sadLeft_bpm[0], &sadRight_bpm[0],
  &disgustLeft_bmp[0], &disgustRight_bmp[0],
  &neutralLeft_bpm[0], &neutralRight_bpm[0]
};

WhaleEyes::WhaleEyes(){}

void WhaleEyes::init(){
  leftEye.begin(0x71);  // pass in the address
  rightEye.begin(0x70);  // pass in the address

  leftEye.setBrightness(15);
  leftEye.setRotation(1);
  rightEye.setBrightness(15);
  rightEye.setRotation(1);
}

void WhaleEyes::setEmotion(short idx){
    byte s=idx*2;
    writeMatrix(bpmIdx[s], bpmIdx[s+1]);
}

void WhaleEyes::writeMatrix(uint8_t left_bmp[], uint8_t right_bmp[]){
  leftEye.clear();
  //(x, y, bitmap, width, height, color)
  leftEye.drawBitmap(0, 0, left_bmp, 8, 8, LED_ON);
  leftEye.writeDisplay();
  
  rightEye.clear();
  //(x, y, bitmap, width, height, color)
  rightEye.drawBitmap(0, 0, right_bmp, 8, 8, LED_ON);
  rightEye.writeDisplay();
}

