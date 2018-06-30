#include "Arduino.h"
#include "WhaleController.h"

bool emotionChanged;

#define LDR_TRIGGERVALUE 700 //the higher the darker
WhaleController *whaleControllerInstance;

//Wrapper function for ISR
WhaleController::WhaleController(){
	whaleControllerInstance = this;
}

void WhaleController::init(){
  whaleEyes.init();
  whaleRGB.init(LED1_R, LED2_R, LED2_G, LED2_B, LED3_R, LED3_1_G, LED3_1_B);
  whaleFins.init(SERVO_PIN);
  whaleSound.init();
  whaleRTC.init();
  whaleFSM.init();

  stopEmotionsTime = 0;
  emotionChanged = false;
  nextEmotion = 0;
  nextDuration = 1000;
  antiBounceDuration = millis();

  //BLUETOOTH
  Serial3.begin(9600);
  Serial3.println("Press enter to begin the setup!");
  delay(1000);
  Serial3.setTimeout(30000); // Timeout for waiting for command from the user.
  
  initButton(PUSHBUTTON_PIN);
  initPir(PIR_PIN);
  
  //set the whale to stay off
  whaleFSM.setGlobalState(102, &nextEmotion, &nextDuration);
  
  //DEBUG ONLY
  // delay(3000);
  // emotionChanged = true;
  // whaleFSM.setGlobalState(100, &nextEmotion, &nextDuration);
}

//////////////////////// ISR ////////////////////////////////////////////////////////

void buttonChanged(){
	Serial.println("buttonChanged");
	cli();
  if (whaleControllerInstance){
    whaleControllerInstance->buttonHandler();
  }
  sei();
}

void WhaleController::buttonHandler(){
	if(millis() - 1000 > antiBounceDuration){
		antiBounceDuration = millis();
		Serial.println("enter Button");
		emotionChanged = true;
		whaleFSM.buttonPressed(&nextEmotion, &nextDuration);
	}
}

// void pirChanged(){
// 	cli();
//   	if (whaleControllerInstance){
//       	whaleControllerInstance->pirHandler();
//   	}
//   	sei();
// }

//////////////////////// Methods ////////////////////////////////////////////////////////

void WhaleController::setEmotion(short emotion, unsigned long int duration){
	whaleEyes.setEmotion(emotion);
	whaleRGB.setEmotion(emotion);
	whaleFins.setEmotion(emotion);
	whaleSound.setEmotion(emotion);
	TIMSK2 = (1 << TOIE2);
	TIMSK4 = (1 << TOIE4);
	// Serial.println(millis());
	this->stopEmotionsTime = millis() + duration;
	// Serial.println(stopEmotionsTime);
}

void WhaleController::stopEmotions(){
	whaleFins.stopFins();
	TIMSK2 = (0 << TOIE2);
	TIMSK4 = (0 << TOIE4);
}

void WhaleController::initButton(int pin){
	this->btn_pin = pin;
	pinMode(this->btn_pin, INPUT);
	attachInterrupt(digitalPinToInterrupt(this->btn_pin), buttonChanged, FALLING);
}

void WhaleController::initLdr(int pin){
	this->ldr_pin = pin;
}

void WhaleController::initPir(int pin){
	this->pir_pin = pin;
	pinMode(this->pir_pin, INPUT);
	// attachInterrupt(digitalPinToInterrupt(this->pir_pin), pirChanged, CHANGE);
}


void WhaleController::routine(){
	//check if we have to change emotion
	Serial.println(emotionChanged);
	if(emotionChanged){
		Serial.println("enter set emotion");
		emotionChanged=false;
		Serial.println(emotionChanged);
	  setEmotion(nextEmotion, nextDuration);
	}

	//check emotion time has elapsed
	if(this->stopEmotionsTime > 0 && millis() > this->stopEmotionsTime){
		Serial.println("enter elapsed");
		cli();
	    this->stopEmotions();
	    emotionChanged=true;
	    whaleFSM.emotionIsOver(&nextEmotion, &nextDuration);
		sei();
	}

	//check PIR
	if (digitalRead(this->pir_pin)) { // check if the input is HIGH
	   if (pirState == LOW) {
	   	// we have just turned on
	   	Serial.println("enter pir");
			emotionChanged = true;
			whaleFSM.motionDetected(&nextEmotion, &nextDuration);
	      // We only want to print on the output change, not state
	      pirState = HIGH;
	   }
	}
	else {
	  if (pirState == HIGH){
	  	// We only want to print on the output change, not state
	   	pirState = LOW;
	  }
	}

	//check LDR
	if(analogRead(this->ldr_pin) > LDR_TRIGGERVALUE){
		Serial.println("enter LDR");
		emotionChanged = true;
		whaleFSM.lightOff(&nextEmotion, &nextDuration);
	}
}

void WhaleController::secondaryRoutine(){
	//check bed and wakeup times
	if(whaleRTC.getCurrHour() == whaleRTC.getBedTimeHour() && whaleRTC.getCurrMinute() == whaleRTC.getBedTimeMinute()){
	    Serial.println("enter bed hour");
	    emotionChanged = true;
	    whaleFSM.setGlobalState(100, &nextEmotion, &nextDuration);
	    Serial.println(nextEmotion);
	}
	else if(whaleRTC.getCurrHour() == whaleRTC.getWakeupTimeHour() && whaleRTC.getCurrMinute() == whaleRTC.getWakeupTimeMinute()){
		Serial.println("enter wakeup");
		emotionChanged = true;
		whaleFSM.setGlobalState(101, &nextEmotion, &nextDuration);
	}
	//FSM will be in charge of getting back to OFF state (i.e. 102)
}


void WhaleController::bluetoothRoutine(){
	//ON THE CELLPHONE TERMINAL, THE ENTER KEY MUST SEND \r\n
	
	//check bluetooth data in
	if (Serial3.available()){
	  String data = Serial3.readStringUntil('\n'); 
		int temp, temp2;
		if(data != NULL){	
			Serial3.println("Hello!\r\n");
			
			while(1) // Just to be possible to break the setup cycle at any time.
			{
				Serial3.println("\r\nPlease choose what you want to do(Just type the number and than press enter):\r\n\n1: Set current time\r\n2: Set bed time\r\n3: Set wake-up time\r\n4: Show current time\r\n5: Show bedtime\r\n6: Show wake-up time\r\n7: Leave\r\n");
				data = Serial3.readStringUntil('\n');
				
				if(data.indexOf('\r') == -1) //If the user doesn't type anything within the timeout
				{
					Serial3.println("Sorry, I'm leaving the setup...");
					break;
				}
				if(data=="1\r"){ //Current time setup
					Serial3.println("Please, type the hour and than, press enter\r\n");
					data = Serial3.readStringUntil('\n');
				
					if(data.indexOf('\r') == -1) //If the user doesn't type anything within the timeout
					{
						Serial3.println("Sorry, I'm leaving the setup...");
						break;
					}
					temp = data.toInt();
					
					if(temp >= 0 && temp < 24)
					{
						Serial3.println("Please, type the minute and than, press enter\r\n");
						data = Serial3.readStringUntil('\n');
					
						if(data.indexOf('\r') == -1) //If the user doesn't type anything within the timeout
						{
							Serial3.println("Sorry, I'm leaving the setup...");
							break;
						}
						temp2 = data.toInt();
						
						if(temp2 >= 0 && temp2 < 60)
						{
							whaleRTC.setCurrTime((byte)temp, (byte)temp2);
							Serial3.println("Time adjusted correctly!\r\n");
						}
						else
						{
							Serial3.println("Invalid number!\r\n");					
						}
					}
					else
					{
						Serial3.println("Invalid number!\r\n");					
					}
						
				}
				else if(data=="2\r"){ //Bedtime setup
					Serial3.println("Please, type the hour and than, press enter\r\n");
					data = Serial3.readStringUntil('\n');
				
					if(data.indexOf('\r') == -1) //If the user doesn't type anything within the timeout
					{
						Serial3.println("Sorry, I'm leaving the setup...");
						break;
					}
					temp = data.toInt();
					
					if(temp >= 0 && temp < 24)
					{
						whaleRTC.setBedTimeHour((byte)temp); 
						
						Serial3.println("Please, type the minute and than, press enter\r\n");
						data = Serial3.readStringUntil('\n');
					
						if(data.indexOf('\r') == -1) //If the user doesn't type anything within the timeout
						{
							Serial3.println("Sorry, I'm leaving the setup...");
							break;
						}
						temp = data.toInt();
						
						if(temp >= 0 && temp < 60)
						{
							whaleRTC.setBedTimeMinute((byte)temp);
							Serial3.println("Bedtime adjusted correctly!\r\n");
						}
						else
						{
							Serial3.println("Invalid number!\r\n");					
						}
					}
					else
					{
						Serial3.println("Invalid number!\r\n");					
					}
					
				}
				else if(data=="3\r"){  //Wake-up time setup
				
					Serial3.println("Please, type the hour and than, press enter\r\n");
					data = Serial3.readStringUntil('\n');
				
					if(data.indexOf('\r') == -1) //If the user doesn't type anything within the timeout
					{
						Serial3.println("Sorry, I'm leaving the setup...");
						break;
					}
					temp = data.toInt();
					
					if(temp >= 0 && temp < 24)
					{
						whaleRTC.setWakeupTimeHour((byte)temp); 
						
						Serial3.println("Please, type the minute and than, press enter\r\n");
						data = Serial3.readStringUntil('\n');
					
						if(data.indexOf('\r') == -1) //If the user doesn't type anything within the timeout
						{
							Serial3.println("Sorry, I'm leaving the setup...");
							break;
						}
						temp = data.toInt();
						
						if(temp >= 0 && temp < 60)
						{
							whaleRTC.setWakeupTimeMinute((byte)temp);
							Serial3.println("Wake-up time adjusted correctly!\r\n");
						}
						else
						{
							Serial3.println("Invalid number!\r\n");					
						}
					}
					else
					{
						Serial3.println("Invalid number!\r\n");					
					}
				}
				else if(data=="4\r"){  //Get current time
					Serial3.println("The current time is: " + String(whaleRTC.getCurrHour()) + ":" + String(whaleRTC.getCurrMinute()) + "\r\n");		
				}
				else if(data=="5\r"){  //Get bed time
					Serial3.println("The current bedtime is at: " + String(whaleRTC.getBedTimeHour()) + ":" + String(whaleRTC.getBedTimeMinute()) + "\r\n");
				}
				else if(data=="6\r"){  //Get wake-up time
					Serial3.println("The current wake-up time is at: " + String(whaleRTC.getWakeupTimeHour()) + ":" + String(whaleRTC.getWakeupTimeMinute()) + "\r\n");
				}
				else if(data=="7\r"){  //Leave the setup  
					break;
				}
				else
					Serial3.println("Invalid number!\r\n");	
					
			}
			
			Serial3.println("Bye bye!");
		}
	}
}