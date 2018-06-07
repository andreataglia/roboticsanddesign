#include "Arduino.h"
#include "WhaleFins.h"

WhaleFins::WhaleFins(){}

void WhaleFins::init(int servo_pin){
  this->_pin = servo_pin;
  pinMode(this->_pin, OUTPUT);
}

void WhaleFins::setEmotion(short idx)
{
	int servo_speed = 400;
	int angle = 180;
  	myservo.write(0);              // tell servo to go to position angle
    delay(servo_speed); 
   	myservo.write(angle);              // tell servo to go to position angle
    delay(servo_speed);
}

