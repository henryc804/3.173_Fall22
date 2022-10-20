#include <ESP_CapSense.h>

CapacitiveSensor   cs_4_33 = CapacitiveSensor(4,33);

float xn1 = 0;
float yn1 =0;
int k = 0;

void setup() {
  cs_4_33.set_CS_AutocaL_Millis(0xFFFFFFFF);
  Serial.begin (115200);
}

void loop() {
  int xn =  cs_4_33.capacitiveSensor(30);


  // Compute the filtered signal
  float yn = 0.969*yn1 + 0.0155*xn + 0.0155*xn1;

  delay(1);
  xn1 = xn;
  yn1 = yn;

  if(k % 3 == 0) {
  // Output
    Serial.print (2*xn);
    Serial.print(" ");
    Serial.println(2*yn);
  }
  k = k+1;
}
