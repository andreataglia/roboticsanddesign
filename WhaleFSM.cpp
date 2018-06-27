#include "Arduino.h"
#include "WhaleFSM.h"

//Global States
#define SLEEPSTATE 		100
#define WAKEUPSTATE 	101
#define IDLESTATE 		102

//Emotions
#define JOY         	1
#define FEAR        	2
#define ANGER       	3
#define SAD         	4
#define DISGUST     	5
#define NEUTRAL     	6
#define IDLE 					-1

//Aditional Emotion/States
#define JOY_IR				11
#define JOY_BTN				12
#define NEUTRAL_IR  	61
#define NEUTRAL_BTN 	62
#define NEUTRAL_LIGHT	63

#define MINUTE 				60000

WhaleFSM::WhaleFSM(){}

void WhaleFSM::init(){
  currGlobalState = IDLESTATE;
  currState = 0;
}

void WhaleFSM::motionDetected(short* nextEmotion, unsigned long int* nextDuration){
	if(currGlobalState == SLEEPSTATE && (currState == NEUTRAL_IR || currState == FEAR)){
  	currState = JOY_IR;
  	*nextEmotion = JOY;
  	*nextDuration = 3000;
  }
  Serial.println(currState);
}

void WhaleFSM::lightOff(short* nextEmotion, unsigned long int* nextDuration){
  if(currGlobalState == SLEEPSTATE && currState == NEUTRAL_LIGHT){
  	currGlobalState == IDLESTATE;
  	currState = NEUTRAL;
  }
  Serial.println(currState);
}

void WhaleFSM::buttonPressed(short* nextEmotion, unsigned long int* nextDuration){
  if(currGlobalState == SLEEPSTATE){
  	if(currState == NEUTRAL_BTN || currState == SAD || currState == ANGER){
  			*nextEmotion = JOY;
  			*nextDuration = 8000;
  			currState = JOY_BTN;
  	}
  	Serial.println(currState);
  }
}


void WhaleFSM::emotionIsOver(short* nextEmotion, unsigned long int* nextDuration){
	if(currGlobalState == SLEEPSTATE){

		switch(currState){
			
			case NEUTRAL_IR:
				*nextEmotion = FEAR;
  			*nextDuration = 1 * MINUTE;
				currState = FEAR;
  			break;

  		case NEUTRAL_BTN:
				*nextEmotion = SAD;
  			*nextDuration = 2 * MINUTE;
  			currState = SAD;
  			break;

  		case NEUTRAL_LIGHT:
				*nextEmotion = SAD;
  			*nextDuration = 2 * MINUTE;
  			currState = SAD;
  			break;

  		case JOY_IR:
				*nextEmotion = NEUTRAL;
				//need to take time from RTC
  			*nextDuration = 5 * MINUTE;
  			currState = NEUTRAL_BTN;
  			break;

  		case JOY_BTN:
				*nextEmotion = NEUTRAL;
  			*nextDuration = 3 * MINUTE;
  			currState = NEUTRAL_BTN;
  			break;

  		case SAD:
				*nextEmotion = ANGER;
  			*nextDuration = 3 * MINUTE;
  			currState = ANGER;
  			break;

  		case ANGER:
				*nextEmotion = ANGER;
  			*nextDuration = 3 * MINUTE;
  			currState = ANGER;
  			break;

  		case FEAR:
				*nextEmotion = NEUTRAL;
  			*nextDuration = 5 * MINUTE;
  			currState = NEUTRAL_IR;
  			break;

  		case IDLE:
				*nextEmotion = NEUTRAL;
  			*nextDuration = 5 * MINUTE;
  			currState = NEUTRAL_IR;
  			break;
		}
		Serial.println(currState);
	}
}

void WhaleFSM::setGlobalState(short newState, short* nextEmotion, unsigned long int* nextDuration){
    currGlobalState = newState;
    if(newState == SLEEPSTATE){
    	currState = IDLE;
    	*nextEmotion = NEUTRAL;
    	*nextDuration = 1 * MINUTE;
    }
}








