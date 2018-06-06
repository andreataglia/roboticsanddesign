#include "Arduino.h"
#include "WhaleEyes.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#define EXPRESSIONS 7

Adafruit_8x8matrix leftEye = Adafruit_8x8matrix();
Adafruit_8x8matrix rightEye = Adafruit_8x8matrix();

uint8_t
  activation_bpm[]=
  { B11111111,
    B11100111,
    B10011001,
    B10111101,
    B01100110,
    B11100111,
    B11111111,
    B11111111 },
//joy  
  joy_bpm[]=
  { B11111111,
    B11111111,
    B11100111,
    B11011011,
    B10111101,
    B10111101,
    B11111111,
    B11111111 },
//joy
  fearLeft_bmp[] =
  { B11111111,
    B11111111,
    B10000001,
    B11110011,
    B11110111,
    B11101111,
    B11111111,
    B11111111 },
  fearRight_bmp[] =
  { B11111111,
    B11111111,
    B10000001,
    B11001111,
    B11101111,
    B11110111,
    B11111111,
    B11111111 },
//joy
  angerLeft_bpm[] =
  { B11111111,
    B10011111,
    B10101111,
    B10110011,
    B10111101,
    B10000000,
    B11111111,
    B11111111 },
  angerRight_bpm[] =
  { B11111111,
    B11111001,
    B11110101,
    B11001101,
    B10111101,
    B00000001,
    B11111111,
    B11111111 },
//joy
  sadLeft_bpm[] =
  { B11111111,
    B11111111,
    B11111111,
    B11111101,
    B11111101,
    B11110001,
    B10000011,
    B00111111 },
  sadRight_bpm[] =
  { B11111111,
    B11111111,
    B11111111,
    B10111111,
    B10111111,
    B10001111,
    B11000001,
    B11111100 },
//joy
  disgustLeft_bmp[] =
  { B11110011,
    B11101111,
    B11011111,
    B10110001,
    B11110001,
    B11110001,
    B11111111,
    B11111111 },
  disgustRight_bmp[] =
  { B11001111,
    B11110111,
    B11111011,
    B10001101,
    B10001111,
    B10001111,
    B11111111,
    B11111111 },
//joy
  neutralLeft_bpm[] = 
  { B11111111,
    B11100011,
    B11000011,
    B11010011,
    B11000011,
    B11000011,
    B11111111,
    B11111111 },
  neutralRight_bpm[] = 
  { B11111111,
    B11000111,
    B11000011,
    B11001011,
    B11000011,
    B11000011,
    B11111111,
    B11111111 };

uint8_t* bpmIdx[EXPRESSIONS*2] = {
  &activation_bpm[0], &activation_bpm[0],
  &joy_bpm[0], &joy_bpm[0],
  &fearLeft_bmp[0], &fearRight_bmp[0],
  &angerLeft_bpm[0], &angerRight_bpm[0],
  &sadLeft_bpm[0], &sadRight_bpm[0],
  &disgustLeft_bmp[0], &disgustRight_bmp[0],
  &neutralLeft_bpm[0], &neutralRight_bpm[0]
};

WhaleEyes::WhaleEyes()
{
  leftEye.begin(0x71);  // pass in the address
  rightEye.begin(0x70);  // pass in the address

  leftEye.setBrightness(15);
  leftEye.setRotation(1);
  rightEye.setBrightness(15);
  rightEye.setRotation(1);
}

void WhaleEyes::setEmotion(byte idx){
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

