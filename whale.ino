#include "WhaleController.h"

WhaleController whaleController;

void setup() {
  Serial.begin(9600);

  whaleController.init();
  whaleController.initButton(PUSHBUTTON_PIN);
  whaleController.initPir(PIR_PIN);

  Serial.println("Started"); 
  Serial.flush();
  //whaleSound.setEmotion(2);
  // whaleController.setEmotion(2, 5000);
}

void loop() {
  delay(1000);
  whaleController.routine();
}
