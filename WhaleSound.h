#ifndef WhaleSound_h
#define WhaleSound_h

#include "Arduino.h"

class WhaleSound
{
  	public:
    	WhaleSound();
    	void init();
    	void setEmotion(short);
    	void stopSound();
	private:
};

#endif
