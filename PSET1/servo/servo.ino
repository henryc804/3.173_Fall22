/**
 * https://www.arduino.cc/reference/en/libraries/esp32servo/
 * All pin numbers are allowed, but only pins 2,4,12-19,21-23,25-27,32-33 are recommended.
 */

#include <ESP32Servo.h>

Servo servo1;
// Published values for SG90 servos; adjust if needed
int minUs = 1000;
int maxUs = 2000;

int servo1Pin = 12;

void setup() {
  ESP32PWM::allocateTimer(0);
  servo1.setPeriodHertz(50);      // Standard 50hz servo
  
  servo1.attach(servo1Pin, minUs, maxUs);

  Serial.begin(115200);
}

void loop() {
  Serial.println("Starting at 0 deg");
  servo1.write(0);
  delay(1000);
  Serial.println("Going to 180 deg");
  servo1.write(180);
  delay(1000);
  Serial.println("Back to 0 deg");
  servo1.write(0);
  delay(1000);
  Serial.println("Going to 90 deg");
  servo1.write(90);
  delay(1000);
  
  Serial.println("Sweeping from 0 to 180");
  for (int pos = 0; pos <= 180; pos += 1) { // sweep from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo1.write(pos);
    delay(50);             // waits 20ms for the servo to reach the position
  }
  for (int pos = 180; pos >= 0; pos -= 1) { // sweep from 180 degrees to 0 degrees
    // in steps of 1 degree
    servo1.write(pos);
    delay(50);             // waits 20ms for the servo to reach the position
  }
} 
