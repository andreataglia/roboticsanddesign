#ifndef WhaleSound_h
#define WhaleSound_h

#include "Arduino.h"

class WhaleSound
{
  	public:
    	WhaleSound();
    	void init(short, short);
    	void setEmotion(short);
	private:
		short speaker_pin;
		short sd_cs_pin;
};

#endif
