#include "Arduino.h"
#include "WhaleSound.h"
#include <SD.h>
#include <SPI.h>
#include <TMRpcm.h>

#define NUMSOUNDS 6

TMRpcm audioPlayer;

char* playedSound = ".wav";
int soundLenght[NUMSOUNDS] = {
	1000,		//Activation
	10000,	//Joy
	11000,	//Fear
	9000,		//Anger
	8000,		//Sad
	8000		//Disgust
};

WhaleSound::WhaleSound(){}

void WhaleSound::init(short speaker, short sd){
	this->speaker_pin = speaker;
	this->sd_cs_pin = sd;

	//SD
	if(!SD.begin(49)) { 
		Serial.println("SD fail");
	}
  	//Audio Starts
	audioPlayer.speakerPin = this->speaker_pin; 
	audioPlayer.setVolume(7);
 	audioPlayer.quality(0);
}

void WhaleSound::setEmotion(short idx){
	if(audioPlayer.isPlaying())
		audioPlayer.disable();

	playedSound = idx + playedSound;
  	audioPlayer.play("1.wav");
}

