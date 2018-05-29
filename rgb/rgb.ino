#include <LEDFader.h>

#define LED_NUM 3
#include <Servo.h>

// 6 LEDs (perhaps 2 RGB LEDs)
LEDFader leds[LED_NUM] = {
  LEDFader(7),
  LEDFader(6),
  LEDFader(5)
};

Servo myservo;

byte color[][4] = { 
 {255,0,0},
 {150,0,0},
 {200,0,0},
 {100,200,0}
};
int c = 0;

int pos = 0;    // variable to store the servo position
int servo_speed = 400;
int angle = 180;

void setup() {
  leds[0].fade(100, 1000);
  leds[1].fade(100, 1000);
  leds[2].fade(100, 1000);  
  myservo.attach(9);
}

void loop() {

  // Update all LEDs and start new fades if any are done
  for (byte i = 0; i < LED_NUM; i++) {
    LEDFader *led = &leds[i];
    led->update();

    // Set new fade
    if (led->is_fading() == false) {
      int duration = servo_speed; // between 1 - 3 seconds

      // Up
      if (led->get_value() == 0) {
        byte color = random(100, 255);
        led->fade(color, duration);
      }
      // Down
      else {
        led->fade(0, duration);
      }
    }
  }
   myservo.write(0);              // tell servo to go to position in variable 'pos'
    delay(servo_speed); 
   myservo.write(angle);              // tell servo to go to position in variable 'pos'
    delay(servo_speed);
}
