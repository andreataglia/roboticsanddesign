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
  emotionChanged = false;
  currGlobalState = IDLESTATE;
  currState = OFF;
}


bool WhaleFSM::emotionIsOver(short* nextEmotion, unsigned long int* nextDuration){
  emotionChanged = false;
  if(currGlobalState == SLEEPSTATE){

    switch(currState){
      
      case OFF:
        *nextEmotion = NEUTRAL;
        *nextDuration = 0.17 * MINUTE; //10 sec 
        currState = NEUTRAL_IR;
        emotionChanged = true;
        break;
      
      case NEUTRAL_IR:
        *nextEmotion = FEAR;
        *nextDuration = 0.17 * MINUTE; //10 sec
        currState = FEAR;
        emotionChanged = true;
        break;

      case NEUTRAL_BTN:
        *nextEmotion = SAD;
        *nextDuration = 0.17 * MINUTE; //10 sec
        currState = SAD;
        emotionChanged = true;
        break;

      case NEUTRAL_LIGHT:
        *nextEmotion = SAD;
        *nextDuration = 0.5 * MINUTE; //30 sec
        currState = SAD;
        emotionChanged = true;
        break;

      case JOY_IR:
        *nextEmotion = NEUTRAL;
        *nextDuration = 0.25 * MINUTE; //15 sec
        currState = NEUTRAL_BTN;
        emotionChanged = true;
        break;

      case JOY_BTN:
        *nextEmotion = NEUTRAL;
        *nextDuration = 0.2 * MINUTE; //12 sec
        currState = NEUTRAL_LIGHT;
        emotionChanged = true;
        break;

      case SAD:
        *nextEmotion = ANGER;
        *nextDuration = 2 * MINUTE;
        currState = ANGER;
        emotionChanged = true;
        break;

      case ANGER:
        *nextEmotion = ANGER;
        *nextDuration = 2 * MINUTE;
        currState = ANGER;
        emotionChanged = true;
        break;

      case FEAR:
        *nextEmotion = NEUTRAL;
        *nextDuration = 0.5 * MINUTE;
        currState = NEUTRAL_IR;
        emotionChanged = true;
        break;
    }

    Serial.print("Current State: ");
    Serial.println(currState);
    Serial.flush();
  }

  else if(currGlobalState == WAKEUPSTATE){
    switch(currState){

      case OFF:
        *nextEmotion = JOY;
        *nextDuration = 0.17 * MINUTE; //10 sec
        currState = JOY_ALARM;
        emotionChanged = true;
        break;

      case JOY_ALARM:
        *nextEmotion = DISGUST;
        *nextDuration = 0.17 * MINUTE; //10 sec
        currState = DISGUST;
        emotionChanged = true;
        break;

      case NEUTRAL_IR:
        *nextEmotion = ANGER;
        *nextDuration = 2 * MINUTE;
        currState = ANGER;
        emotionChanged = true;
        break;

      case DISGUST:
        *nextEmotion = ANGER;
        *nextDuration = 3 * MINUTE;
        currState = ANGER;
        emotionChanged = true;
        break;

      case ANGER:
        *nextEmotion = ANGER;
        *nextDuration = 2 * MINUTE;
        currState = ANGER;
        emotionChanged = true;
        break;

      case JOY_IR:
        *nextEmotion = OFF;
        *nextDuration = 5 * MINUTE; //Robots has to go off till morning
        currState = OFF;
        currGlobalState == IDLESTATE;
        emotionChanged = true;
        break;
    }

    Serial.print("Current State: ");
    Serial.println(currState);
    Serial.flush();
  }

  else if(currGlobalState == IDLESTATE){
    *nextEmotion = OFF;
    *nextDuration = 20 * MINUTE; //Robots has to go off till morning
    currState = OFF;
    emotionChanged = true;

    Serial.print("Current State: ");
    Serial.println(currState);
    Serial.flush();
  }

  return emotionChanged;
}


bool WhaleFSM::motionDetected(short* nextEmotion, unsigned long int* nextDuration){
  emotionChanged = false;

  if(currGlobalState == SLEEPSTATE && (currState == NEUTRAL_IR || currState == FEAR)){
    *nextEmotion = JOY;
    *nextDuration = 8000;
  	currState = JOY_IR;
    emotionChanged = true;

    Serial.print("Current State: ");
    Serial.println(currState);
    Serial.flush();
  }

  else if(currGlobalState == WAKEUPSTATE && currState == NEUTRAL_IR){
    *nextEmotion = JOY;
    *nextDuration = 8000;
    currState = JOY_IR;
    emotionChanged = true;

    Serial.print("Current State: ");
    Serial.println(currState);
    Serial.flush();
  }

  return emotionChanged;
}


bool WhaleFSM::lightOff(short* nextEmotion, unsigned long int* nextDuration){
  emotionChanged = false;

  if(currGlobalState == SLEEPSTATE && currState == NEUTRAL_LIGHT){
    *nextEmotion = OFF;
    *nextDuration = 5 * MINUTE; //Robots has to go off till morning
    currState = OFF;
  	currGlobalState == IDLESTATE;

    emotionChanged = true;
    
    Serial.print("Current State: ");
    Serial.println(currState);
    Serial.flush();
  }

  return emotionChanged;
}


bool WhaleFSM::buttonPressed(short* nextEmotion, unsigned long int* nextDuration){
  emotionChanged = false;
  if(currGlobalState == SLEEPSTATE){
  	if(currState == NEUTRAL_BTN || currState == SAD || currState == ANGER){
  			*nextEmotion = JOY;
  			*nextDuration = 8000;
  			currState = JOY_BTN;
        emotionChanged = true;
  	}

    Serial.print("Current State: ");
    Serial.println(currState);
    Serial.flush();
  }

  else if(currGlobalState == WAKEUPSTATE){
    if(currState == JOY_ALARM || currState == DISGUST || currState == ANGER){
      *nextEmotion = NEUTRAL;
      *nextDuration = 0.17 * MINUTE; //10 sec
      currState = NEUTRAL_IR;
      emotionChanged = true;
    }

    Serial.print("Current State: ");
    Serial.println(currState);
    Serial.flush();
  }

  return emotionChanged;
}


bool WhaleFSM::setGlobalState(short newState, short* nextEmotion, unsigned long int* nextDuration){
  emotionChanged = true;
  currGlobalState = newState;
  
  if(newState == SLEEPSTATE){
    *nextEmotion = OFF;
    *nextDuration = 0.1 * MINUTE;
    currState = OFF;
  }

  else if(newState == WAKEUPSTATE){
    *nextEmotion = OFF;
    *nextDuration = 0.1 * MINUTE;
    currState = OFF;
  }

  else if(newState == IDLESTATE){
    *nextEmotion = OFF;
    *nextDuration = 20 * MINUTE;
    currState = OFF;
  }

  Serial.print("Current State: ");
  Serial.println(currState);
  Serial.flush();

  return emotionChanged;
}