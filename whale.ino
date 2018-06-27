#include "WhaleController.h"

WhaleController whaleController;

void setup() {
  Serial.begin(9600);

  whaleController.init();
  whaleController.initButton(PUSHBUTTON_PIN);
  whaleController.initPir(PIR_PIN);

  Serial.println("Started"); 
  Serial.flush();
  delay(1000);
}

void loop() {
	for(int i=0; i<60; i++){
  		delay(1000);
  		whaleController.routine();
	}
	whaleController.secondaryRoutine();
}
