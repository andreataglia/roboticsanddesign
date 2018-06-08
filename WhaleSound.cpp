#include "Arduino.h"
#include "WhaleSound.h"
#include <SD.h>
#include <SPI.h>
#include <TMRpcm.h>

#define NUMSOUNDS 6

short SPEAKER_PIN;
short SD_CS_PIN;

TMRpcm audioPlayer;

String playedSound = ".wav";
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
	SPEAKER_PIN = speaker;
	SD_CS_PIN = sd;

	//SD
	if(!SD.begin(49)) { 
		Serial.println("SD fail");
	}
  	//Audio Starts
	audioPlayer.speakerPin = SPEAKER_PIN; 
	audioPlayer.setVolume(7);
 	audioPlayer.quality(0);
}

void WhaleSound::setEmotion(short idx){
	if(audioPlayer.isPlaying())
		audioPlayer.disable();

	playedSound = idx + playedSound;
  audioPlayer.play("playedSound");
}

