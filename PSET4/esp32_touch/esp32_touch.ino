/**
 * Assignment 4: Uses ESP32's built-in capacitive sensor hardware
 * You may uncomment the other lines of code below if you want to see the timing between samples
 */

int reading;
//long start;

void setup() {
  Serial.begin(115200);
}

void loop() {
//  start = millis();
  reading = touchRead(T0);
//  
//  Serial.print(millis() - start);        // check how long it took to read in milliseconds
//  Serial.print("\t");                    // tab character for debug window spacing
  Serial.println(reading);
  
  delay(10);
}
