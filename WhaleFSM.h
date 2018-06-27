#ifndef WhaleFSM_h
#define WhaleFSM_h

#include "Arduino.h"

class WhaleFSM
{
  	public:
	    WhaleFSM();
	    void init();
	    void motionDetected(short* nextEmotion, unsigned long int* nextDuration);
	    void lightOff(short* nextEmotion, unsigned long int* nextDuration);
	    void buttonPressed(short* nextEmotion, unsigned long int* nextDuration);
	    void emotionIsOver(short* nextEmotion, unsigned long int* nextDuration);
	    void setGlobalState(short newState, short* nextEmotion, unsigned long int* nextDuration);
	private:
		short currState;
		short currGlobalState;
};

#endif
