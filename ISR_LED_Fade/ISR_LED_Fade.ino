#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

#define REDPIN 11
#define GREENPIN 10
#define BLUEPIN 9

//Time in milliseconds LED stays fixed on a RGB value during "Color fading"
#define TIMESTEP 50
#define EXPRESSIONS 6
//Hardcoded emotion color matrix row dimension
#define NUMCOLOR 26
#define RGB 3
//Color fading transition Threshold in order to change from a color to an other
#define THRESH 4

/* ---- COLOR VARIABLES ----*/
byte currentColor[RGB] = {0,0,0};
byte crntExprIdx;

/**
* All the colors for all the emotion transitions are hardcoded in matrix
* Colors are represented by the 3 byte values of Red, Green, Blue
*/
byte colors[NUMCOLOR][RGB] = {
  //Activation
  {60,60,60},
  {255,255,255},
  //JOY index=2
  {0, 255, 0},
  {0, 255, 255},
  {0, 255, 120},
  {0, 190, 255},
  {130, 255, 0},
  {0, 255, 255},
  {0, 255, 120},
  {0, 255, 255},
  {130, 255, 0},
  //FEAR index=11
  {80,0,100},
  {160,0,255},
  {100,0,150},
  {255,0,255},
  //ANGER index=15
  {255,0,0},
  {80,40,0},
  {255,0,0},
  {150,20,20},
  //SADNESS index=19
  {64,64,255},
  {150,170,225},
  {70,100,190},
  {0,40,120},
  {150,170,225},
  //DISGUST index=24
  {30,90,50},
  {70,170,115},
};
int fadeTime[NUMCOLOR] = {
  //Activation
  1000, 1000,
  //JOY index=
  1000,1000,1000,1000,1000,1000,1000,1000,1000,
  //FEAR index=
  1000,2000,1500,1000,
  //ANGER index=
  1500,2500,1500,3000,
  //SADNESS index=
  1000,1500,2000,1000,1500,
  //DISGUST index=
  2500,1000
};

/** 
* Emotion orientation in terms of starting and ending color will be provided by external indexes representing matrix row.
* Those indexes will be set by the SetEmotion() function.
*/
byte exprIdxStart[EXPRESSIONS]={0,2,11,15,19,24};
byte exprIdxEnd[EXPRESSIONS]={1,10,14,18,23,25};

/**
* In order to easilly change the colors of the individual transitions, this matrix stores all the RGB-step from a color to the next.
* The RGB-step depends on RGB Source and Destination, individual transition time-to-fade, constant TimeStep.
*/
byte difference[NUMCOLOR][RGB];
byte strtTransitionIdx, endTransitionIdx;


/* ---- CONTROL VARIABLES ----*/
byte i, j, k, dif, flag;
unsigned long int nexttime = 50;


ISR(TIMER2_OVF_vect){
  if (millis() > nexttime) {
    cli();
    Serial.print(currentColor[0]);
    Serial.print(", ");
    Serial.print(currentColor[1]);
    Serial.print(", ");
    Serial.println(currentColor[2]);
    Serial.flush();
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
      
      Serial.print("change ");
      Serial.print(strtTransitionIdx);
      Serial.println(endTransitionIdx);
      Serial.flush();
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
        Serial.print(difference[j][k]);
        Serial.print(',');
      }
      Serial.println(' ');
      Serial.flush();
    }
    for(k=0; k<RGB; k++){
      difference[j][k] = round((float)(abs((int)colors[j][k] - (int)colors[exprIdxStart[i]][k]) * TIMESTEP) / fadeTime[j]);
      Serial.print(difference[j][k]);
      Serial.print(',');
    }
    Serial.println(' ');
    Serial.flush();
  }
  
  Serial.println("---\n");
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
    Serial.println("----change----\n");
    Serial.print("strtTransitionIdx: ");
    Serial.println(strtTransitionIdx);
    Serial.print("endTransitionIdx: ");
    Serial.println(endTransitionIdx);
    Serial.println("--------\n");
    Serial.flush();
    for(k=0; k<RGB; k++){
      currentColor[k] = colors[strtTransitionIdx][k];
    }
    sei();
    delay(16000);
  }
}
