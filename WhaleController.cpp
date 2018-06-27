#include "Arduino.h"
#include "WhaleController.h"

bool stateChanged;

#define LDR_TRIGGERVALUE 700 //the higher the darker
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
  whaleFSM.init();
  stopEmotionsTime = 0;
  stateChanged = false;
  nextEmotion = 0;
  nextDuration = 1000;
  antiBounceDuration = millis();
  //BLUETOOTH
  Serial3.begin(9600);
  initButton(PUSHBUTTON_PIN);
  initPir(PIR_PIN);
  whaleFSM.setGlobalState(100, &nextEmotion, &nextDuration);
  setEmotion(6, 10000);
}

//////////////////////// ISR ////////////////////////////////////////////////////////

void buttonChanged(){
	cli();
  	if (whaleControllerInstance){
    	whaleControllerInstance->buttonHandler();
  	}
  	sei();
}

void WhaleController::buttonHandler(){
	if(millis() - 1000 > antiBounceDuration){
		antiBounceDuration = millis();
		Serial.println(whaleRTC.getBedTimeMinute());
		stateChanged = true;
		whaleFSM.buttonPressed(&nextEmotion, &nextDuration);
	}
	
	// currState++;
	// if(currState > 6){
	//     currState = 0;
	// }
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
	attachInterrupt(digitalPinToInterrupt(this->btn_pin), buttonChanged, FALLING);
}

void WhaleController::initLdr(int pin){
	this->ldr_pin = pin;
}

void WhaleController::initPir(int pin){
	this->pir_pin = pin;
	pinMode(this->pir_pin, INPUT);
	// attachInterrupt(digitalPinToInterrupt(this->pir_pin), pirChanged, CHANGE);
}

void WhaleController::routine(){
	//check if we have to change emotion
	if(stateChanged){
		stateChanged=false;
	    setEmotion(nextEmotion, nextDuration);
	}

	//check emotion time has elapsed
	if(this->stopEmotionsTime > 0 && millis() > this->stopEmotionsTime){
		cli();
	    this->stopEmotions();
	    stateChanged=true;
	    whaleFSM.emotionIsOver(&nextEmotion, &nextDuration);
		sei();
	}

	//check PIR
	if (digitalRead(this->pir_pin)) { // check if the input is HIGH
	    if (pirState == LOW) {
	      // we have just turned on
			stateChanged = true;
			whaleFSM.motionDetected(&nextEmotion, &nextDuration);
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
		stateChanged = true;
		whaleFSM.lightOff(&nextEmotion, &nextDuration);
	}
}

void WhaleController::secondaryRoutine(){
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

	//check bed and wakeup times
	if(whaleRTC.getCurrHour() == whaleRTC.getBedTimeHour() && whaleRTC.getCurrMinute() == whaleRTC.getBedTimeMinute()){
	    whaleFSM.setGlobalState(100, &nextEmotion, &nextDuration);
	}else if(false){
		whaleFSM.setGlobalState(101, &nextEmotion, &nextDuration);
	}
}
