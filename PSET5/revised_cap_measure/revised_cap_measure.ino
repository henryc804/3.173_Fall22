#define readPin   A0
#define sendPin   33
#define resVal    10000000.0F // F to tell compiler this is a float

unsigned long startTime;
unsigned long elapsedTime;
float picoFarads;

void setup() {
  pinMode(sendPin, OUTPUT);     // set chargePin to output
  pinMode(readPin, INPUT);
  digitalWrite(sendPin, LOW);

  Serial.begin(115200);             // initialize serial transmission for debugging
  Serial.println("SETUP DONE");
}

void loop() {
  digitalWrite(sendPin, LOW);
  pinMode(readPin, INPUT);
  pinMode(readPin, OUTPUT);
  digitalWrite(readPin, LOW);
  
  delayMicroseconds(10);
  
  pinMode(readPin, INPUT);
  digitalWrite(sendPin, HIGH);  // beginning charging

  startTime = micros();
  while(analogRead(readPin) < 2588) {}  // 2588 is 63% of full scale 12-bit ADC

  elapsedTime = micros() - startTime;

  picoFarads = (float)elapsedTime / resVal * 1000000;
  Serial.print(elapsedTime);
  Serial.print(" uS   ");
  Serial.print((long)picoFarads);
  Serial.println(" pF");

  delay(1000);
}
