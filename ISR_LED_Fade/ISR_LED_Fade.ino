#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

#define REDPIN 11
#define GREENPIN 10
#define BLUEPIN 9

#define TIMESTEP 50
#define EXPRESSIONS 3
#define NUMCOLOR 16
#define RGB 3
#define THRESH 4

/* ---- COLOR VARIABLES ----*/
byte currentColor[RGB] = {255,0,0};
byte crntExprIdx;

byte colors[NUMCOLOR][RGB] = {
  {0, 255, 0},//JOY index=0
  {0, 255, 255},//1
  {0, 255, 120},//2
  {0, 190, 255},//3
  {130, 255, 0},//4
  {0, 255, 255},//5
  {0, 255, 120},//6
  {0, 255, 255},//7
  {130, 255, 0},//8
  {80,0,100},//FEAR index=9
  {170,0,255},//10
  {100,0,150},//11
  {170,0,255},//12
  {255,0,0},//ANGER index=13
  {80,40,0},//14
  {150,20,20}//15
};
int fadeTime[NUMCOLOR] = {
  //JOY index=0
  1000,1000,1000,1000,1000,1000,1000,1000,1000,
  //FEAR index=9
  1000,1000,1000,1000,
  //ANGER index=13
  1000,1000,1000
};
byte exprIdxStart[EXPRESSIONS]={0, 9, 13};
byte exprIdxEnd[EXPRESSIONS]={8, 12, 15};
byte difference[NUMCOLOR][RGB];
byte strtTransitionIdx, endTransitionIdx;


/* ---- CONTROL VARIABLES ----*/
byte i, j, k, dif, flag;
unsigned long int nexttime = 50;


ISR(TIMER2_OVF_vect){
  if (millis() > nexttime) {
    cli();
//    Serial.print(currentColor[0]);
//    Serial.print(", ");
//    Serial.print(currentColor[1]);
//    Serial.print(", ");
//    Serial.println(currentColor[2]);
//    Serial.flush();
    //write del colore dato in precedenza contenuto in un array
    analogWrite(REDPIN, currentColor[0]);
    analogWrite(GREENPIN, currentColor[1]);
    analogWrite(BLUEPIN, currentColor[2]);
    
    // Calculate new color for next ISR
    for(k = 0; k < RGB; k++){
      if(currentColor[k] != colors[endTransitionIdx][k]){
        if(currentColor[k] < colors[endTransitionIdx][k])
          currentColor[k] += difference[strtTransitionIdx][k];
        else
          currentColor[k] -= difference[strtTransitionIdx][k];
      }
    }

    //check if this fading transition has finished finished
    for(k=0, flag = 0; k<RGB; k++){
      dif = abs(currentColor[k] - colors[endTransitionIdx][k]);
      if(dif>0 && dif<=THRESH)
        flag=1;
    }
    if(flag){
      strtTransitionIdx++;
      if(strtTransitionIdx > exprIdxEnd[crntExprIdx])
        strtTransitionIdx = exprIdxStart[crntExprIdx];
      
      endTransitionIdx++;
      if(endTransitionIdx > exprIdxEnd[crntExprIdx])
        endTransitionIdx = exprIdxStart[crntExprIdx];
      
      for(k=0; k<RGB; k++){
        currentColor[k] = colors[strtTransitionIdx][k];
      }
      
//      Serial.print("change ");
//      Serial.print(strtTransitionIdx);
//      Serial.println(endTransitionIdx);
//      Serial.flush();
    } 
    nexttime = millis() + TIMESTEP;
    sei();
  }
}

void calcDifference(){
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
      difference[j][k] = round((abs((int)colors[j][k] - (int)colors[exprIdxStart[i]][k]) * TIMESTEP) / fadeTime[j]);
//      Serial.print(difference[j][k]);
//      Serial.print(',');
    }
//    Serial.println(' ');
//    Serial.flush();
  }
  
//  Serial.println("---\n");
}

void setup(){
  Serial.begin(9600);
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  calcDifference();
  TIMSK2 = (1 << TOIE2);
}

void loop() {
  for(crntExprIdx = 0; crntExprIdx < EXPRESSIONS; crntExprIdx++){
    cli();
    strtTransitionIdx = exprIdxStart[crntExprIdx];
    endTransitionIdx = exprIdxStart[crntExprIdx]+1;
//    Serial.println("----change----\n");
//    Serial.print("strtTransitionIdx: ");
//    Serial.println(strtTransitionIdx);
//    Serial.print("endTransitionIdx: ");
//    Serial.println(endTransitionIdx);
//    Serial.println("--------\n");
//    Serial.flush();
    for(k=0; k<RGB; k++){
      currentColor[k] = colors[strtTransitionIdx][k];
    }
    sei();
    delay(16000);
  }
}
