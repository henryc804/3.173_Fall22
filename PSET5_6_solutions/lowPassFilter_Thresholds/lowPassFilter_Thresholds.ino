#include <ESP_CapSense.h>

const int sendPin = 4, receivePin = 33, LED = 13;
// resistor between pins 4 (A5) & 33, pin 33 is read pin
CapacitiveSensor sensor = CapacitiveSensor(sendPin,receivePin);

// timer variable [milliseconds]
unsigned long currentTime;
int waitTime = 10000; // 10 seconds

// threshold variables
int minThreshold = 0, hoverThreshold = 0, touchThreshold = 0;
float hoverFactor = 1.5, touchFactor = 2;

// program variables
int numberOfSamples = 30;
const int signalDelay = 100;
const int LONGER = 1, LONGER_FACTOR = 5;
const int SHORTER = 2;
int counter = 0;

int prevChargeTime, prevOutput, prevChargeTime;
float FILTER_COEFF[] = [0.969, 0.0155, 0.0155];

/***********************************************/

void setup()
{
  sensor.set_CS_AutocaL_Millis(0xFFFFFFFF);  // turns off autocalibrate
  Serial.begin(115200);

  setupThresholds();
  setupLED();
}

void loop()                    
{
  // total time to charge from function with set number of samples
  int currentChargeTime = sensor.capacitiveSensor(numberOfSamples);

  // Computing a weighted signal, biased to not shift very much from previous values
  float currentOutput = FILTER_COEFF[0] * prevOutput + FILTER_COEFF[1] * currentChargeTime + FILTER_COEFF[2] * prevChargeTime;

  // Updating the previous values
  prevChargeTime = currentChargeTime;
  prevOutput = currentOutput;

  // Only uses every third data point as the previous 2 are used to calculate the filter data point
  if (counter % 3 == 0) {
    if (currentOutput > touchThreshold) {
      signalToUser(LONGER);
    } else if (currentOutput > hoverThreshold) {
      signalToUser(SHORTER);
    }

    Serial.println(currentOutput);
  }
  counter++;
}

/*************** HELPER FUNCTIONS ****************/

/**
 * Signals to user by flashing a LED fiber
 */
void signalToUser(int type) {
  if (type == LONGER) {
      digitalWrite(LED, HIGH);
      delay(LONGER_FACTOR * signalDelay);
      digitalWrite(LED, LOW);
  } else if (type == SHORTER) {
      digitalWrite(LED, HIGH);
      delay(signalDelay);
      digitalWrite(LED, LOW);

  }

  switch (type) {
    case LONGER:
      digitalWrite(LED, HIGH);
      delay(LONGER_FACTOR * signalDelay);
      digitalWrite(LED, LOW);
      break
    case SHORTER:
      digitalWrite(LED, HIGH);
      delay(signalDelay);
      digitalWrite(LED, LOW);
      break;
  }
}

/**
 * Initializes LED GPIO pin and sets the LED as off initially
 */
void setupFiber() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

/**
 * Throws away the first few seconds of data
 * User should NOT touch the fiber upon startup
 * Then, calculates the hover/touch thresholds by a certain percentage above the noise floor
 */
void setupThresholds() {
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