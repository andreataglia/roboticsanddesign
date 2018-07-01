#ifndef WhaleFSM_h
#define WhaleFSM_h

#include "Arduino.h"

class WhaleFSM
{
  public:
	  WhaleFSM();
	  void init();
    bool motionDetected(short* nextEmotion, unsigned long int* nextDuration);
    bool lightOff(short* nextEmotion, unsigned long int* nextDuration);
    bool buttonPressed(short* nextEmotion, unsigned long int* nextDuration);
    bool emotionIsOver(short* nextEmotion, unsigned long int* nextDuration);
    bool setGlobalState(short newState, short* nextEmotion, unsigned long int* nextDuration);
	private:
		short currState;
		short currGlobalState;
		bool emotionChanged;
};

#endif
