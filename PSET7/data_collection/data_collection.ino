/*
https://github.com/edgeimpulse/example-data-collection-csv
*/

#include <ESP_CapSense.h>

// Capacitive sensor fibers
CapacitiveSensor fiber1 = CapacitiveSensor(4,33);        // resistor between pins 4 (A5) & 33, pin 33 is read pin
CapacitiveSensor fiber2 = CapacitiveSensor(4,15);        // resistor between pins 4 (A5) & 15, pin 15 is read pin

// Constants
const int numReadings = 5;
#define LED                 13      // Built-in LED is on pin 13
#define SAMPLING_TIME       1.5     // in seconds (I found that 1.5 seconds is about the time I do a gesture)
#define SAMPLING_FREQ_HZ    50       // Sampling frequency (Hz)
#define SAMPLING_PERIOD_MS  1000 / SAMPLING_FREQ_HZ   // Sampling period (ms)
#define NUM_SAMPLES         SAMPLING_TIME * SAMPLING_FREQ_HZ

void setup()
{
  fiber1.set_CS_AutocaL_Millis(0xFFFFFFFF);  // turns off autocalibrate
  fiber2.set_CS_AutocaL_Millis(0xFFFFFFFF);  // turns off autocalibrate
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  Serial.begin(115200);
}

void loop()                    
{
  long fiber1Reading;
  long fiber2Reading;
  unsigned long timestamp;
  unsigned long start_timestamp;

  // Begin blinking LED to notify that measurement is about to start again
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);
  digitalWrite(LED, HIGH);
  delay(250);
  digitalWrite(LED, LOW);
  delay(250);
  digitalWrite(LED, HIGH);
  delay(250);
  digitalWrite(LED, LOW);
  delay(250);
  digitalWrite(LED, HIGH);

  // Print header (for EdgeImpulse's CSV format)
  Serial.println("timestamp,chargetimeLeft,chargetimeRight");

  // Record samples in buffer
  start_timestamp = millis();
  for (int index = 0; index < NUM_SAMPLES; index++) {
    // taking timestamp to measure out target frequency
    timestamp = millis();

    // Fiber 1 is left
    fiber1Reading =  fiber1.capacitiveSensor(numReadings);
    // Fiber 2 is right
    fiber2Reading =  fiber2.capacitiveSensor(numReadings);

    Serial.print(timestamp - start_timestamp);
    Serial.print(",");
    Serial.print(fiber1Reading);
    Serial.print(",");
    Serial.println(fiber2Reading);

    // Wait just long enough for our sampling period
    while (millis() < timestamp + SAMPLING_PERIOD_MS);
  }

  Serial.println();

  // Turn off LED to show one measurement is done
  digitalWrite(LED, LOW);

  delay(5000);  // slowing down output
}
