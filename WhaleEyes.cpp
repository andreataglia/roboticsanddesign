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
    B11111111,
    B11000111,
    B10111011,
    B10111011,
    B10111011,
    B11111111,
    B11111111 },
  joyRight_bpm[]=
  { B11111111,
    B11100011,
    B11011101,
    B11011101,
    B11011101,
    B11111111,
    B11111111,
    B00000000 },
//FEAR
  fearLeft_bmp[] =
  { B00000000,
    B11111111,
    B11111111,
    B00000011,
    B11110111,
    B11101110,
    B11011100,
    B11111000 },
  fearRight_bmp[] =
  { B01111111,
    B01111111,
    B01000000,
    B01101111,
    B01110111,
    B00111011,
    B00001111,
    B00000000 },
//ANGER
  angerLeft_bpm[] =
  { B00000000,
    B11111000,
    B10011100,
    B10101111,
    B10110011,
    B10111101,
    B10000000,
    B11111111 },
  angerRight_bpm[] =
  { B00011111,
    B00111001,
    B11110101,
    B11001101,
    B10111101,
    B00000001,
    B11111111,
    B11111111 },
//SAD
  sadLeft_bpm[] =
  { B00000000,
    B00111111,
    B01111011,
    B11111011,
    B11110011,
    B11100111,
    B00011111,
    B01111110 },
  sadRight_bpm[] =
  { B11111100,
    B11011110,
    B11011111,
    B11001111,
    B11100111,
    B11111000,
    B11111111,
    B11111110 },
//DISGUST
  disgustLeft_bmp[] =
  { B00000000,
    B00111111,
    B01100111,
    B11100011,
    B11010011,
    B11001011,
    B11000111,
    B01111111 },
  disgustRight_bmp[] =
  { B11111100,
    B11100110,
    B11000111,
    B11001011,
    B11010011,
    B11100011,
    B11111111,
    B11111111 },
//NEUTRAL
  neutralLeft_bpm[] = 
  { B00111111,
    B01100011,
    B11001111,
    B10011111,
    B10110011,
    B11100011,
    B11100111,
    B11111111 },
  neutralRight_bpm[] = 
  { B11000110,
    B11110011,
    B11111001,
    B11001101,
    B11000111,
    B11100111,
    B11111111,
    B11111111 };

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

