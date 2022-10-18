#include <ESP_CapSense.h>

CapacitiveSensor   cs_4_33 = CapacitiveSensor(4,33);        // resistor between pins 4 (A5) & 33, pin 33 is read pin

void setup()
{
  cs_4_33.set_CS_AutocaL_Millis(0xFFFFFFFF);  // turns off autocalibrate
  Serial.begin(115200);
}

void loop()                    
{
  long total =  cs_4_33.capacitiveSensor(500);

  Serial.println(total);

  delay(10);  // slowing down output
}
