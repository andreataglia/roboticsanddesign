#include "Arduino.h"
#include "WhaleFSM.h"

//Global States
#define SLEEPSTATE 100
#define WAKEUPSTATE 101
#define NEUTRALSTATE 102

//Emotions
#define ACTIVATION  0
#define JOY         1
#define FEAR        2
#define ANGER       3
#define SAD         4
#define DISGUST     5
#define NEUTRAL     6

#define minute 6000

WhaleFSM::WhaleFSM(){}

void WhaleFSM::init(){
  currGlobalState = WAKEUPSTATE;
  currState = 0;
}

void WhaleFSM::motionDetected(short* nextEmotion, unsigned long int* nextDuration){
    // switch (currGlobalState) {
    //     case SLEEPSTATE:
    //       switch (currState) {
    //           case 0:
    //             // do something
    //             break;
    //           case 1:
    //             // do something
    //             break;
              
    //           default:
    //             // do something
    //       }
    //       break;
        
    //     case WAKEUPSTATE:
    //       switch (currState) {
    //           case 0:
    //             currState = 1;
    //             whaleController.setEmotion(JOY, (0,5 * minute));
    //             break;
    //           case 1:
    //             currState = 0;
    //             whaleController.setEmotion(ANGER, (0,5 * minute));
    //             break;
              
    //           default:
    //             // do something
    //       }
          
    //       break;
    //     default:
    //       do something
    // }
    *nextEmotion = 3;
    *nextDuration = 5000;
}

void WhaleFSM::lightOff(short* nextEmotion, unsigned long int* nextDuration){
    //TODO
    *nextEmotion = FEAR;
    *nextDuration = 9000;
}

void WhaleFSM::buttonPressed(short* nextEmotion, unsigned long int* nextDuration){
    //TODO
    *nextEmotion = 4;
    *nextDuration = 5000;
}


void WhaleFSM::emotionIsOver(short* nextEmotion, unsigned long int* nextDuration){
    *nextEmotion = ANGER;
    *nextDuration = 10000;
  //TODO
}

void WhaleFSM::setGlobalState(short newState){
    currGlobalState = newState;
}