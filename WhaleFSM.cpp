#include "Arduino.h"
#include "WhaleFSM.h"

//Global States
#define SLEEPSTATE 		100
#define WAKEUPSTATE 	101
#define IDLESTATE 		102

//Emotions
#define OFF           0
#define JOY           1
#define FEAR          2
#define ANGER         3
#define SAD           4
#define DISGUST       5
#define NEUTRAL       6

//Aditional Emotion/States
#define JOY_IR				11
#define JOY_BTN				12
#define JOY_ALARM     13
#define JOY_WOKEUP    14
#define NEUTRAL_IR  	61
#define NEUTRAL_BTN 	62
#define NEUTRAL_LIGHT	63

#define MINUTE 				60000

WhaleFSM::WhaleFSM(){}

void WhaleFSM::init(){
  currGlobalState = IDLESTATE;
  currState = 0;
}


void WhaleFSM::emotionIsOver(short* nextEmotion, unsigned long int* nextDuration){
  if(currGlobalState == SLEEPSTATE){

    switch(currState){
      
      case OFF:
        *nextEmotion = NEUTRAL;
        *nextDuration = 0.5 * MINUTE; //30 sec 
        currState = NEUTRAL_IR;
        break;
      
      case NEUTRAL_IR:
        *nextEmotion = FEAR;
        *nextDuration = 0.25 * MINUTE; //15 sec
        currState = FEAR;
        break;

      case NEUTRAL_BTN:
        *nextEmotion = SAD;
        *nextDuration = 0.5 * MINUTE; //30 sec
        currState = SAD;
        break;

      case NEUTRAL_LIGHT:
        *nextEmotion = SAD;
        *nextDuration = 0.5 * MINUTE; //30 sec
        currState = SAD;
        break;

      case JOY_IR:
        *nextEmotion = NEUTRAL;
        //need to take time from RTC
        *nextDuration = 1 * MINUTE;
        currState = NEUTRAL_BTN;
        break;

      case JOY_BTN:
        *nextEmotion = NEUTRAL;
        *nextDuration = 1 * MINUTE;
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
        *nextDuration = 0.5 * MINUTE;
        currState = NEUTRAL_IR;
        break;

    }
    Serial.println(currState);
  }

  else if(currGlobalState == WAKEUPSTATE){
    switch(currState){

      case OFF:
        *nextEmotion = JOY;
        *nextDuration = 2 * MINUTE;
        currState = JOY_ALARM;
        break;

      case JOY_ALARM:
        *nextEmotion = DISGUST;
        *nextDuration = 0.5 * MINUTE;
        currState = DISGUST;
        break;

      case NEUTRAL_IR:
        *nextEmotion = ANGER;
        *nextDuration = 3 * MINUTE;
        currState = ANGER;
        break;

      case DISGUST:
        *nextEmotion = ANGER;
        *nextDuration = 3 * MINUTE;
        currState = ANGER;
        break;

      case ANGER:
        *nextEmotion = ANGER;
        *nextDuration = 3 * MINUTE;
        currState = ANGER;
        break;

      case JOY_IR:
        *nextEmotion = OFF;
        *nextDuration = 5 * MINUTE; //Robots has to go off till morning
        currState = OFF;
        currGlobalState == IDLESTATE;
        break;
    }
  }

  else if(currGlobalState == IDLESTATE){
    *nextEmotion = OFF;
    *nextDuration = 20 * MINUTE; //Robots has to go off till morning
    currState = OFF;
  }
}


void WhaleFSM::motionDetected(short* nextEmotion, unsigned long int* nextDuration){
	if(currGlobalState == SLEEPSTATE && (currState == NEUTRAL_IR || currState == FEAR)){
    *nextEmotion = JOY;
    *nextDuration = 4000;
  	currState = JOY_IR;
  }

  else if(currGlobalState == WAKEUPSTATE && currState == NEUTRAL_IR){
    *nextEmotion = JOY;
    *nextDuration = 7000;
    currState = JOY_IR;
  }
  
  Serial.println(currState);
}


void WhaleFSM::lightOff(short* nextEmotion, unsigned long int* nextDuration){
  if(currGlobalState == SLEEPSTATE && currState == NEUTRAL_LIGHT){
    *nextDurationtEmotion = OFF;
    *nextDuration = 5 * MINUTE; //Robots has to go off till morning
    currState = OFF;
  	currGlobalState == IDLESTATE;
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
  }

  else if(currGlobalState == WAKEUPSTATE){
    if(currState == JOY_ALARM || currState == DISGUST || currState == ANGER){
      *nextEmotion = NEUTRAL;
      *nextDuration = 1 * MINUTE;
      currState = NEUTRAL_IR;
    }
  }
  Serial.println(currState);
}


void WhaleFSM::setGlobalState(short newState, short* nextEmotion, unsigned long int* nextDuration){
    currGlobalState = newState;
    if(newState == SLEEPSTATE){
      *nextEmotion = OFF;
      *nextDuration = 0.3 * MINUTE;
    	currState = OFF;
    }
    Serial.println(currState);
}