#include "Arduino.h"
#include "WhaleRGB.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>


//Time in milliseconds LED stays fixed on a RGB value during "Color fading"
#define TIMESTEP 50
#define EXPRESSIONS 7
//Hardcoded emotion color matrix row dimension
#define NUMCOLOR 33
#define RGB 3

byte currentColor[RGB] = {0,0,0};
byte exprIdx;

/**
 * All the colors for all the emotion transitions are hardcoded in matrix
 * Colors are represented by the 3 byte values of Red, Green, Blue 
 */
byte colors[NUMCOLOR][RGB] = {
  //Activation
  {60,60,60},//0
  {255,255,255},//1
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
  {160,0,255},//12
  {100,0,150},//13
  {255,0,255},//14
  //ANGER
  {255,0,0},//15
  {80,40,0},//16
  {255,0,0},//17
  {150,20,20},//18
  //SADNESS
  {64,64,255},//19
  {150,170,225},//20
  {70,100,190},//21
  {0,40,120},//22
  {150,170,225},//23
  //DISGUST
  {30,90,50},//24
  {70,170,115},//25
  //NEUTRAL
  {240,240,240},//26
  {80,80,80},//27
  {80,80,80},//28
  {0,0,0},//29
  {0,0,0},//30
  {80,80,80},//31
  {80,80,80},//32
};
int fadeTime[NUMCOLOR] = {
  //Activation index=0
  1000, 1000,
  //JOY index=2
  1000,1000,1000,1000,1000,1000,1000,1000,1000,
  //FEAR index=11
  1000,2000,1500,1000,
  //ANGER index=15
  1500,2500,1500,3000,
  //SADNESS index=19
  1000,1500,2000,1000,1500,
  //DISGUST index=24
  2500,1000,
  //NEUTRAL index=26
  5000,1000,1500,3000,1000,1500,3000
};

/**
 * Emotion orientation in terms of starting and ending color will be provided by external indexes representing matrix row.
 * Those indexes will be set by the SetEmotion() function.
 */
byte exprIdxStart[EXPRESSIONS]={0,2,11,15,19,24,26};
byte exprIdxEnd[EXPRESSIONS]={1,10,14,18,23,25,32};
/**
 * In order to easilly change the colors of the individual transitions, this matrix stores all the RGB-step from a color to the next.
 * The RGB-step depends on RGB Source and Destination, individual transition time-to-fade, constant TimeStep.
 */
byte difference[NUMCOLOR][RGB];
byte strtTransitionIdx, endTransitionIdx;

          ///////////////////////////
          //---Control Variables---//
          ///////////////////////////
byte i, j, k, dif, flag;
unsigned long int nexttime = 50;

int REDPIN1;
int GREENPIN1;
int BLUEPIN1;

WhaleRGB::WhaleRGB(int LED1_R,int LED1_G,int LED1_B)
{
  pinMode(LED1_R, OUTPUT);
  pinMode(LED1_G, OUTPUT);
  pinMode(LED1_B, OUTPUT);
//  pinMode(LED2_R, OUTPUT);
//  pinMode(LED2_G, OUTPUT);
//  pinMode(LED2_B, OUTPUT);
//  pinMode(LED3_R, OUTPUT);
//  pinMode(LED3_G, OUTPUT);
//  pinMode(LED3_B, OUTPUT);
  _pin = LED1_R;
  REDPIN1 = LED1_R;
  GREENPIN1 = LED1_G;
  BLUEPIN1 = LED1_B;
  calcDifference();
  TIMSK2 = (1 << TOIE2);
}

void WhaleRGB::setEmotion(byte idx)
{
  cli();
  exprIdx=idx;
  strtTransitionIdx = exprIdxStart[exprIdx];
  endTransitionIdx = exprIdxStart[exprIdx]+1;
  for(k=0; k<RGB; k++){
    currentColor[k] = colors[strtTransitionIdx][k];
  }
  sei();
//  for(exprIdx = 0; exprIdx < EXPRESSIONS; exprIdx++){
//    cli();
//    strtTransitionIdx = exprIdxStart[exprIdx];
//    endTransitionIdx = exprIdxStart[exprIdx]+1;
////    Serial.println("\n---- CHANGE EMOTION ----\n");
////    Serial.print("strtTransitionIdx: ");
////    Serial.println(strtTransitionIdx);
////    Serial.print("endTransitionIdx: ");
////    Serial.println(endTransitionIdx);
////    Serial.println("--------\n");
////    Serial.flush();
//    for(k=0; k<RGB; k++){
//      currentColor[k] = colors[strtTransitionIdx][k];
//    }
//    sei();
//    delay(16000);
//  }
}

void WhaleRGB::dash()
{
  digitalWrite(_pin, HIGH);
  delay(1000);
  digitalWrite(_pin, LOW);
  delay(250);
}

ISR(TIMER2_OVF_vect){
  if (millis() > nexttime) {
    //disable Global interrupt in order to prevent other ISR
    cli();
//    Serial.print(currentColor[0]);
//    Serial.print(", ");
//    Serial.print(currentColor[1]);
//    Serial.print(", ");
//    Serial.println(currentColor[2]);
//    Serial.flush();
    
    //RGB write from the previous color
    analogWrite(REDPIN1, currentColor[0]);
    analogWrite(GREENPIN1, currentColor[1]);
    analogWrite(BLUEPIN1, currentColor[2]);
    
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
    for(k=0, flag = 0; k<RGB; k++){
      dif = abs(currentColor[k] - colors[endTransitionIdx][k]);
      if(dif>0 && dif<=difference[strtTransitionIdx][k])
        flag=1;
    }

    //if transition complete Transition indexes are incremented by 1, in order to load the new transition.
    if(flag){
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
//      Serial.print("\n change ");
//      Serial.print(strtTransitionIdx);
//      Serial.println(endTransitionIdx);
//      Serial.flush();
    } 
    nexttime = millis() + TIMESTEP;
    sei();
  }
}

void WhaleRGB::calcDifference(){
  for(i=0; i<EXPRESSIONS; i++){
    for(j=exprIdxStart[i]; j<exprIdxEnd[i]; j++){
      for(k=0; k<RGB; k++){
        difference[j][k] = round((float)(abs((int)colors[j][k] - (int)colors[j+1][k]) * (int)TIMESTEP) / fadeTime[j]);
//        Serial.print(difference[j][k]);
//        Serial.print(',');
      }
//      Serial.println(' ');
//      Serial.flush();
    }
    for(k=0; k<RGB; k++){
      difference[j][k] = round((float)(abs((int)colors[j][k] - (int)colors[exprIdxStart[i]][k]) * TIMESTEP) / fadeTime[j]);
//      Serial.print(difference[j][k]);
//      Serial.print(',');
    }
//    Serial.println(' ');
//    Serial.flush();
  }
//  Serial.println("---\n");
}
