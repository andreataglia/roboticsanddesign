#include "Arduino.h"
#include "WhaleController.h"

int currState;
bool stateChanged;


#define LDR_TRIGGERVALUE 800 //the higher the darker
WhaleController *whaleControllerInstance;

//Wrapper function for ISR
WhaleController::WhaleController(){
	whaleControllerInstance = this;
}


void WhaleController::init(){
  whaleEyes.init();
  whaleRGB.init(LED1_R, LED2_R, LED2_G, LED2_B, LED3_R, LED3_1_G, LED3_1_B);
  whaleFins.init(SERVO_PIN);
  whaleSound.init();
  whaleRTC.init();
  whaleFSM.init(this);
  stopEmotionsTime = 0;
  currState = 0;
  stateChanged = false;
  //BLUETOOTH
  Serial3.begin(9600);
}

//////////////////////// ISR ////////////////////////////////////////////////////////

void buttonChanged(){
	cli();
  	if (whaleControllerInstance){
    	whaleControllerInstance->buttonHandler();
  	}
  	sei();
}

// void pirChanged(){
// 	cli();
//   	if (whaleControllerInstance){
//       	whaleControllerInstance->pirHandler();
//   	}
//   	sei();
// }

//////////////////////// Methods ////////////////////////////////////////////////////////

void WhaleController::setEmotion(short emotion, unsigned long int duration){
	whaleEyes.setEmotion(emotion);
	whaleRGB.setEmotion(emotion);
	whaleFins.setEmotion(emotion);
	whaleSound.setEmotion(emotion);
	TIMSK2 = (1 << TOIE2);
	TIMSK4 = (1 << TOIE4);
	this->stopEmotionsTime = millis() + duration;
}

void WhaleController::stopEmotions(){
	// whaleRGB.setEmotion(emotion, duration);
	// whaleEyes.setEmotion(emotion);
	// whaleFins.setEmotion(emotion);
	TIMSK2 = (0 << TOIE2);
	TIMSK4 = (0 << TOIE4);
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
	// attachInterrupt(digitalPinToInterrupt(this->pir_pin), pirChanged, CHANGE);
}

void WhaleController::buttonHandler(){
	whaleFSM.buttonPressed();
	Serial.println(currState);
	currState++;
	if(currState > 6){
	    currState = 0;
	}
	stateChanged = true;
}

// void WhaleController::pirHandler(){
//   	if(digitalRead(this->pir_pin)){
// 		Serial.println("ciao mbare");
//  	}else{
//  		Serial.println("niente...");
//  	}
// }

void WhaleController::routine(){
	//check emotion time has elapsed
	if(this->stopEmotionsTime > 0 && millis() > this->stopEmotionsTime){
		cli();
	    this->stopEmotions();
	    whaleFSM.emotionIsOver();
		sei();
	}

	//check bluetooth data in
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

	//check PIR
	if (digitalRead(this->pir_pin)) { // check if the input is HIGH
	    if (pirState == LOW) {
	      // we have just turned on
	      	whaleFSM.motionDetected();
	      // We only want to print on the output change, not state
	      	pirState = HIGH;
	    }
	} else {
	    if (pirState == HIGH){
	      	// We only want to print on the output change, not state
	    	pirState = LOW;
	    }
	}

	//check LDR
	if(analogRead(this->ldr_pin) > LDR_TRIGGERVALUE){
		whaleFSM.lightOff();
	}
}
