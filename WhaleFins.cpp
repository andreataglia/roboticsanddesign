#include "Arduino.h"
#include "WhaleFins.h"

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
}

void WhaleFins::setEmotion(short idx)
{
  	servo.write(0);   
}

ISR(TIMER3_OVF_vect){
  if (millis() > nextServoMove && servoActiveEmotion){    
    servo.write(servoAngle - servoAngle * servoClockwise);
		servoClockwise = 1 - servoClockwise;  
    nextServoMove = millis() + servoTimeStep;
 	}
}

