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
//  Serial.println("secondary routine");
  whaleController.secondaryRoutine();
	for(int i=0; i<5; i++){
		for(int j=0; j<11; j++){
			delay(1000);
//      Serial.println("routine");
			whaleController.routine();
		}
//    Serial.println("Blue routine");
		whaleController.bluetoothRoutine();
	}
}
