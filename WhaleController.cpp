#include "Arduino.h"
#include "WhaleController.h"

#define LDR_TRIGGERVALUE 400
WhaleController *whaleControllerInstance;

WhaleController::WhaleController(){
	whaleControllerInstance = this;
}

void WhaleController::init(WhaleRGB whaleRGB, WhaleSound whaleSound, WhaleFins whaleFins, WhaleEyes whaleEyes){
  this->whaleRGB = whaleRGB;
  this->whaleSound = whaleSound;
  this->whaleFins = whaleFins;
  this->whaleEyes = whaleEyes;
  state = IDLE;
}

//////////////////////// ISR ////////////////////////////////////////////////////////

void buttonChanged(){
  if (whaleControllerInstance)
      whaleControllerInstance->buttonHandler();
}

void pirChanged(){
  if (whaleControllerInstance){
      whaleControllerInstance->pirHandler();
  }
}

//////////////////////// Methods ////////////////////////////////////////////////////////

void WhaleController::setEmotion(short emotion, unsigned long int duration){
	whaleRGB.setEmotion(emotion, duration);
}

void WhaleController::initButton(int pin){
	this->btn_pin = pin;
	pinMode(this->btn_pin, OUTPUT);
	attachInterrupt(digitalPinToInterrupt(this->btn_pin), buttonChanged, CHANGE);
}

void WhaleController::initLdr(int pin){
	this->ldr_pin = pin;
}

void WhaleController::initPir(int pin){
	this->ldr_pin = pin;
	pinMode(this->pir_pin, INPUT);
	attachInterrupt(digitalPinToInterrupt(this->pir_pin), buttonChanged, CHANGE);
}

void WhaleController::routine(){
  if(analogRead(this->ldr_pin) > LDR_TRIGGERVALUE){
  	//TODO
  }
}

void WhaleController::buttonHandler(){
  this->setEmotion(3, 1000);
}

void WhaleController::pirHandler(){
  	if(digitalRead(this->pir_pin)){
		//TODO
 	}
}

