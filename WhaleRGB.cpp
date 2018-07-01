#include "Arduino.h"
#include "WhaleRGB.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>


//Time in milliseconds LED stays fixed on a RGB value during "Color fading"
#define TIMESTEP 50
#define TIME_THRESH 50
#define EXPRESSIONS 7
//Hardcoded emotion color matrix row dimension
#define NUMCOLOR 28
#define RGB 3

byte currentColor[RGB] = {0,0,0};

/**
 * All the colors for all the emotion transitions are hardcoded in matrix
 * Colors are represented by the 3 byte values of Red, Green, Blue 
 */
byte colors[NUMCOLOR][RGB] = {
  //OFF
  {0,0,0},//0
  {0,0,0},//1
  //JOY
  {0, 255, 0},//2
  {0, 255, 255},//3
  {0, 255, 120},//4
  {0, 190, 255},//5
  {130, 255, 0},//6
  {0, 255, 255},//7
  {0, 255, 120},//8
  {0, 255, 255},//9
  {130, 255, 0},//10
  //FEAR
  {80,0,100},//11
  {215,110,230},//12
  {170,0,255},//13
  {240,0,140},//14
  //ANGER
  {255,0,0},//15
  {80,80,0},//16
  {255,0,0},//17
  {150,50,50},//18
  //SAD
  {0,0,255},//19
  {30,90,225},//20
  {70,30,165},//21
  {0,30,120},//22
  {0,100,225},//23
  //DISGUST
  {0,60,20},//24
  {40,200,20},//25
  //NEUTRAL
  {255,255,255},//26
  {60,60,60},//27
};
int fadeTime[NUMCOLOR] = {
  //off index=0
  10000, 10000,
  //JOY index=2
  1200,1200,1200,1200,1200,1200,1200,1200,1200,
  //FEAR index=11
  1500,1500,1500,1500,
  //ANGER index=15
  1500,2500,1500,3000,
  //SAD index=19
  3000,2000,3000,2000,3000,
  //DISGUST index=24
  3500,2000,
  //NEUTRAL index=26
  6000,6000
};

/**
 * Emotion orientation in terms of starting and ending color will be provided by external indexes representing matrix row.
 * Those indexes will be set by the SetEmotion() function.
 */
byte exprIdxStart[EXPRESSIONS]={0,2,11,15,19,24,26};
byte exprIdxEnd[EXPRESSIONS]={1,10,14,18,23,25,27};

/**
 * In order to easilly change the colors of the individual transitions, this matrix stores all the RGB-step from a color to the next.
 * The RGB-step depends on RGB Source and Destination, individual transition time-to-fade, constant TimeStep.
 */
byte difference[NUMCOLOR][RGB];
short strtTransitionIdx, endTransitionIdx;
short exprIdx;

          ///////////////////////////
          //---Control Variables---//
          ///////////////////////////
byte i, j, k, dif;
unsigned long int nexttime;
unsigned long int duration;
bool colorReached;


int REDPIN1;
int REDPIN2;
int GREENPIN2;
int BLUEPIN2;
int REDPIN3;
int GREENPIN3_1;
int BLUEPIN3_1;

WhaleRGB::WhaleRGB(){}

void WhaleRGB::init(int LED1_R,int LED2_R,int LED2_G,int LED2_B,int LED3_R,int LED3_G,int LED3_B){
  
  REDPIN1 = LED1_R;
  REDPIN2 = LED2_R;
  GREENPIN2 = LED2_G;
  BLUEPIN2 = LED2_B;
  REDPIN3 = LED3_R;
  GREENPIN3_1 = LED3_G;
  BLUEPIN3_1 = LED3_B;
  pinMode(REDPIN1, OUTPUT);
  pinMode(REDPIN2, OUTPUT);
  pinMode(GREENPIN2, OUTPUT);
  pinMode(BLUEPIN2, OUTPUT);
  pinMode(REDPIN3, OUTPUT);
  pinMode(GREENPIN3_1, OUTPUT);
  pinMode(BLUEPIN3_1, OUTPUT);

  calcDifference();
  nexttime = 50;
}

void WhaleRGB::setEmotion(short idx){
  exprIdx=idx;
  strtTransitionIdx = exprIdxStart[exprIdx];
  endTransitionIdx = exprIdxStart[exprIdx]+1;
  for(k=0; k<RGB; k++){
    currentColor[k] = colors[strtTransitionIdx][k];
  }
}

ISR(TIMER2_OVF_vect){
  if(millis() > nexttime){
      
  	//RGB write from the previous color
    analogWrite(REDPIN1, currentColor[0]);
    analogWrite(REDPIN2, currentColor[0]);
    analogWrite(REDPIN3, currentColor[0]);
    analogWrite(GREENPIN2, currentColor[1]);
    analogWrite(GREENPIN3_1, currentColor[1]);
    analogWrite(BLUEPIN2, currentColor[2]);
    analogWrite(BLUEPIN3_1, currentColor[2]);
    
    //Debug ONLY
    // Serial.println(currentColor[0]);
    // Serial.println(currentColor[1]);
    // Serial.println(currentColor[2]);
    // Serial.println("-------\n");
    // Serial.flush();

    // Calculate new color for next ISR
    for(k = 0; k < RGB; k++){
      if(currentColor[k] != colors[endTransitionIdx][k]){
        if(currentColor[k] < colors[endTransitionIdx][k])
          currentColor[k] += difference[strtTransitionIdx][k];
        else
          currentColor[k] -= difference[strtTransitionIdx][k];
      }
    }
  
    //check if, with this fading transition, 'currentColor' has reached RGB end stored in matrix
    for(k=0, colorReached = false; k<RGB; k++){
      dif = abs(currentColor[k] - colors[endTransitionIdx][k]);
      if(dif>0 && dif<=difference[strtTransitionIdx][k])
        colorReached = true;
    }
  
    //if transition complete Transition indexes are incremented by 1, in order to load the new transition.
    if(colorReached){
      strtTransitionIdx++;
      //if one of the indexes has reached the end for the emotion, the first emotion index is loaded (fading loop)
      if(strtTransitionIdx > exprIdxEnd[exprIdx])
        strtTransitionIdx = exprIdxStart[exprIdx];
      
      endTransitionIdx++;
      if(endTransitionIdx > exprIdxEnd[exprIdx])
        endTransitionIdx = exprIdxStart[exprIdx];
      
      for(k=0; k<RGB; k++){
        currentColor[k] = colors[strtTransitionIdx][k];
      }
    }

    nexttime = millis() + TIMESTEP;
  }
}


void WhaleRGB::calcDifference(){
  for(i=0; i<EXPRESSIONS; i++){
    for(j=exprIdxStart[i]; j<exprIdxEnd[i]; j++){
      for(k=0; k<RGB; k++)
        difference[j][k] = round((float)(abs((int)colors[j][k] - (int)colors[j+1][k]) * (int)TIMESTEP) / fadeTime[j]);
    }
    for(k=0; k<RGB; k++)
      difference[j][k] = round((float)(abs((int)colors[j][k] - (int)colors[exprIdxStart[i]][k]) * TIMESTEP) / fadeTime[j]);
  }
}
