#include "Arduino.h"
#include "WhaleFins.h"

int durations[] = {2000, 300, 300, 200, 1000, 700, 0};
int angles[] = {160, 180, 120, 100, 180, 130, 0};
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

void WhaleFins::setEmotion(short idx)
{
    currEmotionIndex = idx;
  	servo.write(0);   
}

ISR(TIMER4_OVF_vect){
  if (millis() > nextServoMove){    
    servo.write(angles[currEmotionIndex] - angles[currEmotionIndex] * servoClockwise);
		servoClockwise = 1 - servoClockwise;  
    nextServoMove = millis() + durations[currEmotionIndex];
 	}
}

