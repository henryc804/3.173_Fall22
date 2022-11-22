#include <ESP_CapSense.h>

const int sendPin = 4, receivePin = 33;
// resistor between pins 4 (A5) & 33, pin 33 is read pin
CapacitiveSensor sensor = CapacitiveSensor(sendPin,receivePin);

// timer variable [milliseconds]
unsigned long currentTime;
int waitTime = 10000; // 10 seconds

// threshold variables
int minThreshold = 0, hoverThreshold = 0, touchThreshold = 0;
float hoverFactor = 2, touchFactor = 5;

/*******************************/

void setup()
{
  sensor.set_CS_AutocaL_Millis(0xFFFFFFFF);  // turns off autocalibrate
  Serial.begin(115200);

  currentTime = millis();
  while (millis() - currentTime < waitTime) {
    Serial.println("WAIT");
    Serial.println(sensor.capacitiveSensor(numberOfSamples));
  }

  for (int counter = 0; counter < 10; counter++) {
    minThreshold += sensor.capacitiveSensor(numberOfSamples);
  }
  minThreshold /= 10;
  Serial.print("MIN THRESHOLD: ");
  Serial.println(minThreshold);
  
  hoverThreshold = hoverFactor * minThreshold;
  touchThreshold = touchFactor * minThreshold;
}

void loop()                    
{
  // PROGRAM LOGIC GOES HERE
}