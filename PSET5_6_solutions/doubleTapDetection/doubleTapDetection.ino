#include <ESP_CapSense.h>

const int sendPin = 4, receivePin = 33, LED = 13;
// resistor between pins 4 (A5) & 33, pin 33 is read pin
CapacitiveSensor sensor = CapacitiveSensor(sendPin,receivePin);

// timer variable [milliseconds]
unsigned long currentTime, startTime;
int waitTime = 5000; // 5 seconds

const int doubleTapWaitTime = 500;
const int tapDebounceTime = 100;

// threshold variables
int minThreshold = 0, hoverThreshold = 0, touchThreshold = 0;
float hoverFactor = 2, touchFactor = 5;

// program variables
int numberOfSamples = 30;
const int signalDelay = 100;
bool doubleTapFlag = false;

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
  int chargeTime = sensor.capacitiveSensor(numberOfSamples);

  // A tap was detected
  if (chargeTime > touchThreshold) {
    // Wait to allow user to withdraw their hand for another tap
    delay(tapDebounceTime);

    // start timer
    startTime = millis();

    while( (millis() - startTime) < doubleTapWaitTime) {
      chargeTime = sensor.capacitiveSensor(numberOfSamples);
      if (chargeTime > touchThreshold) {
        doubleTapFlag = true;
        break;
      }
    }
    
    if (doubleTapFlag) {
      signalToUser();
      signalToUser();
      signalToUser();
      signalToUser();
      signalToUser();
      // resetting flag
      doubleTapFlag = false;
    }
  }

  Serial.println(chargeTime);
}

/*************** HELPER FUNCTIONS ****************/

/**
 * Signals to user by flashing a LED fiber
 */
void signalToUser() {
  digitalWrite(LED, HIGH);
  delay(signalDelay);
  digitalWrite(LED, LOW);
}

/**
 * Initializes LED GPIO pin and sets the LED as off initially
 */
void setupLED() {
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