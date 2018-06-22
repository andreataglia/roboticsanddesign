#include "Arduino.h"
#include "WhaleController.h"

#define ACTIVATION  0
#define JOY         1
#define FEAR        2
#define ANGER       3
#define SAD         4
#define DISGUST     5
#define NEUTRAL     6

int currState;
bool stateChanged;

#define LDR_TRIGGERVALUE 800
WhaleController *whaleControllerInstance;

WhaleController::WhaleController(){
	whaleControllerInstance = this;
}


void WhaleController::init(){
  whaleEyes.init();
  whaleRGB.init(LED1_R, LED2_R, LED2_G, LED2_B, LED3_R, LED3_1_G, LED3_1_B);
  whaleFins.init(SERVO_PIN);
  whaleSound.init(SPEAKER_PIN, SD_CS_PIN);
  whaleRTC.init();
  stopEmotionsTime = 0;
  currState = 0;
  stateChanged = false;
  //BLUETOOTH
  Serial3.begin(9600);

  whaleEyes.setEmotion(NEUTRAL);
}

//////////////////////// ISR ////////////////////////////////////////////////////////

void buttonChanged(){
	cli();
  	if (whaleControllerInstance){
    	whaleControllerInstance->buttonHandler();
  	}
  	sei();
}

void pirChanged(){
	cli();
  	if (whaleControllerInstance){
      	whaleControllerInstance->pirHandler();
  	}
  	sei();
}

//////////////////////// Methods ////////////////////////////////////////////////////////

void WhaleController::setEmotion(short emotion, unsigned long int duration){
	whaleEyes.setEmotion(emotion);
	whaleRGB.setEmotion(emotion);
	whaleFins.setEmotion(emotion);
	TIMSK2 = (1 << TOIE2);
	TIMSK3 = (1 << TOIE3);
	this->stopEmotionsTime = millis() + duration;
}

void WhaleController::stopEmotions(){
	// whaleRGB.setEmotion(emotion, duration);
	// whaleEyes.setEmotion(emotion);
	// whaleFins.setEmotion(emotion);
	TIMSK2 = (0 << TOIE2);
	TIMSK3 = (0 << TOIE3);
}

void WhaleController::initButton(int pin){
	this->btn_pin = pin;
	pinMode(this->btn_pin, INPUT);
	attachInterrupt(digitalPinToInterrupt(this->btn_pin), buttonChanged, RISING);
}

void WhaleController::initLdr(int pin){
	this->ldr_pin = pin;
}

void WhaleController::initPir(int pin){
	this->pir_pin = pin;
	pinMode(this->pir_pin, INPUT);
	attachInterrupt(digitalPinToInterrupt(this->pir_pin), pirChanged, CHANGE);
}

void WhaleController::buttonHandler(){
	Serial.println(currState);
	currState++;
	if(currState > 6){
	    currState = 0;
	}
	stateChanged = true;
}

void WhaleController::pirHandler(){
  	if(digitalRead(this->pir_pin)){
		Serial.println("ciao mbare");
 	}else{
 		Serial.println("niente...");
 	}
}

void WhaleController::routine(){
	Serial.println(whaleRTC.getCurrMinute());
	Serial.println(whaleRTC.getCurrHour());
	if(stateChanged){
	    setEmotion(currState, 4500);
	    stateChanged = false;
	}
	if(this->stopEmotionsTime > 0 && millis() > this->stopEmotionsTime){
		cli();
	    this->stopEmotions();
		sei();
	}
	if (Serial3.available()){
	   	String data = Serial3.readStringUntil('\n'); 
		if(data != NULL){	
			Serial3.println("ciao! 1:set bed time, 2: set wakeup time, 3: show bedtime, 4:show wakeup time");
	   		Serial3.println(data);
	   		if(data=="1,20\n"){  
	    		Serial3.println("perfetto");
	    		// this->setEmotion(4, 2000);
	   		}
		}
	}
	// if(analogRead(this->ldr_pin) > LDR_TRIGGERVALUE){
	//   	// Serial.println(analogRead(this->ldr_pin));
	//   	Serial.println("pir changed");
	// 	currState++;
	// 	if(currState > 6){
	// 	    currState = 0;
	// 	}
	// }
}
