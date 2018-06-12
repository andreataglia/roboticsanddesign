#define SPEAKER_PIN       5
#define PIR_PIN           19
#define PUSHBUTTON_PIN    18
#define LDR_PIN           A0
#define SD_CS_PIN         49
#define SERVO_PIN         46

#define LED1_R            13
#define LED1_G            44
#define LED1_B            45

#define LED2_R            8
#define LED2_G            9
#define LED2_B            10

#define LED3_R            4
#define LED3_G            6
#define LED3_B            7

#include "WhaleController.h"

WhaleEyes whaleEyes;
WhaleRGB whaleRGB;
WhaleFins whaleFins;
WhaleSound whaleSound;
WhaleRTC whaleRTC;
WhaleController whaleController;

#define ACTIVATION  0
#define JOY         1
#define FEAR        2
#define ANGER       3
#define SAD         4
#define DISGUST     5
#define NEUTRAL     6
#define IDLE_STATE  7

void setup() {
  Serial.begin(9600);
  
   //BLUETOOTH
  Serial3.begin(9600);

  whaleEyes.init();
  whaleRGB.init(LED1_R, LED1_G, LED1_B, LED2_R, LED2_G, LED2_B, LED3_R, LED3_G, LED3_B);
  whaleFins.init(SERVO_PIN);
  whaleSound.init(SPEAKER_PIN, SD_CS_PIN);
  whaleRTC.init();
  whaleController.init(whaleRGB, whaleSound, whaleFins, whaleEyes);

  Serial.println("Started"); 
  Serial.flush();
}

void loop() {
  for(short i=0; i<7; i++){
    whaleController.setEmotion(i, 9000);
    delay(9000);
  }
}
