#include "WhaleController.h"

WhaleController whaleController;

void setup() {
  Serial.begin(9600);

  whaleController.init();

  Serial.println("Started"); 
  Serial.flush();
  delay(1000);
}

void loop(){
  whaleController.secondaryRoutine();
	for(int i=0; i<60; i++){
		for(int j = 0; j < 10; j++){
			delay(1000);
			whaleController.routine();
		}
		whaleController.bluetoothRoutine();
	}
}
