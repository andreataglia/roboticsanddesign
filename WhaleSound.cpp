#include "Arduino.h"
#include "WhaleSound.h"
#include "DFRobotDFPlayerMini.h"

#define NUMSOUNDS 6

DFRobotDFPlayerMini audioPlayer;

int soundLenght[NUMSOUNDS] = {
	1000,		//Activation
	10000,	//Joy
	11000,	//Fear
	9000,		//Anger
	8000,		//Sad
	8000		//Disgust
};

WhaleSound::WhaleSound(){}

void WhaleSound::init(){
 	Serial2.begin(9600);
 	if (!audioPlayer.begin(Serial2)) {  //Use softwareSerial to communicate with mp3.
  		Serial.println(F("Unable to begin:"));
    	Serial.println(F("1.Please recheck the connection!"));
    	Serial.println(F("2.Please insert the SD card!"));
    	while(true);
  	}
  	Serial.println(F("DFPlayer Mini online."));
  
  	audioPlayer.volume(30);  //Set volume value. From 0 to 30
}

void WhaleSound::setEmotion(short idx){
	audioPlayer.play(idx);  //Play mp3 by position
}

void WhaleSound::stopSound(){
	audioPlayer.pause();
}

