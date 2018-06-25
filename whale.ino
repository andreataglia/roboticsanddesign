#include "WhaleController.h"

WhaleController whaleController;

void setup() {
  Serial.begin(9600);

  whaleController.init();
  whaleController.initButton(PUSHBUTTON_PIN);
  whaleController.initPir(PIR_PIN);

  Serial.println("Started"); 
  Serial.flush();
  // whaleSound.setEmotion(2);
  delay(4000);
  whaleController.setEmotion(2, 50000);
}

void loop() {
  // delay(15000);
  // whaleController.setEmotion(3, 10000);
  // delay(15000);
  // whaleController.setEmotion(4, 10000);
  // delay(15000);
  // whaleController.setEmotion(5, 10000);
  delay(1000);
  whaleController.routine();
}
