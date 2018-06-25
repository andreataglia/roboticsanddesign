#ifndef WhaleFSM_h
#define WhaleFSM_h

#include "Arduino.h"
#include "WhaleController.h"

class WhaleFSM
{
  	public:
	    WhaleFSM();
	    void init(const WhaleController*);
	    void motionDetected();
	    void lightOff();
	    void buttonPressed();
	    void emotionIsOver();
	private:
		short currState;
		short currGlobalState;
		WhaleController *whaleController;
};

#endif
