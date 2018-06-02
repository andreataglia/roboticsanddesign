#include <avr/io.h>
#include <avr/interrupt.h>

#define REDPIN 11
#define GREENPIN 10
#define BLUEPIN 9

#define TIMESTEP 50
#define N 6
#define RGB 3

/* ---- COLOR VARIABLES ----*/
byte currentColor[RGB] = {0,0,0};
byte colors[N][RGB] = {
  {0, 0, 0},
  {255, 255, 255},
  {0, 0, 0},
  {180, 20, 100},
  {120, 120, 0},
  {0, 255, 225}
};
byte difference[N][RGB];
byte startColorIndex, endColorIndex;
int fadeTime[N] = {3000,3000,3000,3000,3000,3000};

/* ---- CONTROL VARIABLES ----*/
byte i, j;
unsigned long int nexttime = 0;


void calcDifference(){
  for(i=0; i<N-1; i++){
    for(j=0; j<RGB; j++){
      difference[i][j] = (abs(colors[i][j] - colors[i+1][j]) * TIMESTEP) / fadeTime[i];
      Serial.print(difference[i][j]);
    }
    Serial.println(' ');
  }
  
  for(j=0; j<RGB; j++){
    difference[i][j] = (abs(colors[i][j] - colors[0][j]) * TIMESTEP) / fadeTime[i];
    Serial.println(difference[i][j]);
  }
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

    //go to next color after fading transition finished
    if(abs(currentColor[0] - colors[endColorIndex][0]) < 5 || abs(currentColor[1] - colors[endColorIndex][1]) < 5  || abs(currentColor[2] - colors[endColorIndex][2]) < 5){
      
      Serial.print("change ");
      Serial.print(startColorIndex);
      Serial.println(endColorIndex);
      startColorIndex++;
      endColorIndex++;
      
      if(endColorIndex > N)
        endColorIndex=0;
      if(startColorIndex > N)
        startColorIndex=0;
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
