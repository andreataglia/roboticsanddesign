#include "Arduino.h"
#include "WhaleFins.h"

// OFF       0
// JOY       1
// FEAR      2
// ANGER     3
// SAD       4
// DISGUST   5
// NEUTRAL   6

int durations[] = {0, 300, 200, 200, 1400, 500, 0};
int angles[] = {0, 180, 100, 100, 180, 130, 0};
short currEmotionIndex;

Servo servo;
unsigned long int nextServoMove;
int servoAngle;
int servoTimeStep;
short servoClockwise;
bool servoActiveEmotion;

WhaleFins::WhaleFins(){}

void WhaleFins::init(int servo_pin){
  this->_pin = servo_pin;
  servo.attach(this->_pin);
  nextServoMove = 50;
  servoAngle = 180;
  servoClockwise = 0;
  servoActiveEmotion = true;
  servoTimeStep = 300;
  servo.write(0);
  currEmotionIndex = 0;
}

void WhaleFins::setEmotion(short idx){
  currEmotionIndex = idx;
  servo.write(90);
}

void WhaleFins::stopFins(){
  servo.write(90);
}

ISR(TIMER4_OVF_vect){
  if (millis() > nextServoMove){    
    servo.write(angles[currEmotionIndex] - angles[currEmotionIndex] * servoClockwise);
		servoClockwise = 1 - servoClockwise;  
    nextServoMove = millis() + durations[currEmotionIndex];
 	}
}

