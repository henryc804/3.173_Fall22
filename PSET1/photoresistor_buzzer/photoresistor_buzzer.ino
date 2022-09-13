int sensorValue;
int sensorLow = 1023;
int sensorHigh = 0;

void setup() {
  Serial.begin(115200);
  while (millis() < 5000) {
    sensorValue = analogRead(A0);
    if (sensorValue > sensorHigh) {
      sensorHigh = sensorValue;
    }
    if (sensorValue < sensorLow) {
      sensorLow = sensorValue;
    }
  }
  
  ledcAttachPin(8, 0);
  
  Serial.println("Starting");
}

void loop() {
  sensorValue = analogRead(A0);
  Serial.print("Sensor value: ");
  Serial.println(sensorValue);
  
  int pitch = map(sensorValue,sensorLow,sensorHigh, 3000, 8000);
  Serial.print("Pitch: ");
  Serial.println(pitch);

  // Argument 1 is channel, which can always be 0 for our purposes
  ledcWriteTone(0, pitch);
  
  delay(10);
}
