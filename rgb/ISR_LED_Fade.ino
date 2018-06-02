#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

#define REDPIN 11
#define GREENPIN 10
#define BLUEPIN 9

#define TIMESTEP 50
#define N 6
#define RGB 3
#define THRESH 4

/* ---- COLOR VARIABLES ----*/
byte currentColor[RGB] = {0,0,0};
byte colors[N][RGB] = {
  {0, 255, 0},
  {0, 255, 255},
  {0, 255, 120},
  {0, 190, 255},
  {130, 255, 0},
  {0, 255, 255}
};
byte difference[N][RGB];
byte startColorIndex, endColorIndex;
int fadeTime[N] = {1000,1000,1000,1000,1000,1000};

/* ---- CONTROL VARIABLES ----*/
byte i, j, dif, flag;
unsigned long int nexttime = 0;


void calcDifference(){
  for(i=0; i<N-1; i++){
    for(j=0; j<RGB; j++){
      difference[i][j] = round((abs((int)colors[i][j] - (int)colors[i+1][j]) * (int)TIMESTEP) / fadeTime[i]);
      Serial.print(difference[i][j]);
      Serial.print(',');
    }
    Serial.println(' ');
  }
  
  for(j=0; j<RGB; j++){
    difference[i][j] = round((abs((int)colors[i][j] - (int)colors[0][j]) * TIMESTEP) / fadeTime[i]);
    Serial.print(difference[i][j]);
  }
  Serial.println(' ');
  Serial.println("---\n");
}

ISR(TIMER2_OVF_vect){
  if (millis() > nexttime) {
    Serial.print(currentColor[0]);
    Serial.print(", ");
    Serial.print(currentColor[1]);
    Serial.print(", ");
    Serial.println(currentColor[2]);
    //write del colore dato in precedenza contenuto in un array
    analogWrite(REDPIN, currentColor[0]);
    analogWrite(GREENPIN, currentColor[1]);
    analogWrite(BLUEPIN, currentColor[2]);
    
    // Calculate new color for next ISR
    for(j=0; j<RGB; j++){
      if(currentColor[j] < colors[endColorIndex][j])
        currentColor[j] += difference[startColorIndex][j];
      else
        currentColor[j] -= difference[startColorIndex][j];
    }

    //check if this fading transition has finished finished
    for(j=0, flag = 0; j<RGB; j++){
      dif = abs(currentColor[j] - colors[endColorIndex][j]);
      if(dif>0 && dif<THRESH)
        flag=1;
    }
    if(flag){  
      startColorIndex++;
      for(j=0; j<RGB; j++){
        currentColor[j] = colors[startColorIndex][j];
      }
      endColorIndex++;
      
      if(endColorIndex == N)
        endColorIndex=0;
      if(startColorIndex == N)
        startColorIndex=0;

      Serial.print("change ");
      Serial.print(startColorIndex);
      Serial.println(endColorIndex);
    } 
    nexttime = millis() + TIMESTEP;
  }
}


void setup(){
  // put your setup code here, to run once:
  TIMSK2 = (1 << TOIE2);
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  Serial.begin(9600);
  sei();
  calcDifference();
  startColorIndex=0;
  endColorIndex=1;
}

void loop() {
  
}
