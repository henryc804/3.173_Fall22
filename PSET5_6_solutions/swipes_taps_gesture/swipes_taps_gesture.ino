/*
This code technically doesn't implement logic to make sure if a gesture is detected, another gesture is not also allowed to be detected.
For example, you will notice that you can do left swipe and right double tap almost simultaneously.
A quick fix would be to add delays and adding some logic checks on the flags.
We'll leave this as an exercise for the reader :)
*/

#include <ESP_CapSense.h>

const int sendPin = 4, leftReceivePin = 33, rightReceivePin = 15, LED = 13;
// resistor between pins 4 (A5) & 33, pin 33 is read pin
CapacitiveSensor leftFiber = CapacitiveSensor(sendPin, leftReceivePin);
// resistor between pins 4 (A5) & 33, pin 15 is read pin
CapacitiveSensor rightFiber = CapacitiveSensor(sendPin, rightReceivePin);

// timer variable [milliseconds]
unsigned long currentTime, startTime;
int waitTime = 5000; // 5 seconds

const int secondTapWait = 500;
const int tapDebounceTime = 100;

float leftFiberChargeTime, rightFiberChargeTime;

// threshold variables
int leftMinThreshold = 0, rightMinThreshold = 0, leftHoverThreshold = 0, leftTouchThreshold = 0, rightHoverThreshold = 0, rightTouchThreshold = 0;
float hoverFactor = 2, touchFactor = 5;

// program variables
int numberOfSamples = 30;
const int signalDelay = 100;
bool rightFiberDoubleTapFlag = false, leftFiberDoubleTapFlag = false, rightSwipeFlag = false, leftSwipeFlag = false;

/***********************************************/

void setup()
{
  leftFiber.set_CS_AutocaL_Millis(0xFFFFFFFF);  // turns off autocalibrate
  rightFiber.set_CS_AutocaL_Millis(0xFFFFFFFF);  // turns off autocalibrate
  Serial.begin(115200);

  setupThresholds();
  setupLED();
}

void loop()                    
{
  leftFiberChargeTime = leftFiber.capacitiveSensor(numberOfSamples);
  rightFiberChargeTime = rightFiber.capacitiveSensor(numberOfSamples);

  if (leftFiberChargeTime > leftTouchThreshold && rightFiberChargeTime < rightTouchThreshold) {
    // Wait to allow user to withdraw their hand for another tap
    delay(tapDebounceTime);

    // start timer
    startTime = millis();

    while( (millis() - startTime) < secondTapWait) {
      leftFiberChargeTime = leftFiber.capacitiveSensor(numberOfSamples);
      rightFiberChargeTime = rightFiber.capacitiveSensor(numberOfSamples);

      if (leftFiberChargeTime > leftTouchThreshold) {
        leftFiberDoubleTapFlag = true;
        break;
      }
      if (rightFiberChargeTime > rightTouchThreshold) {
        rightSwipeFlag = true;
        break;
      }
    }
  }

  if (rightFiberChargeTime > rightTouchThreshold && leftFiberChargeTime < leftTouchThreshold) {
    // Wait to allow user to withdraw their hand for another tap
    delay(tapDebounceTime);

    // start timer
    startTime = millis();

    while( (millis() - startTime) < secondTapWait) {
      leftFiberChargeTime = leftFiber.capacitiveSensor(numberOfSamples);
      rightFiberChargeTime = rightFiber.capacitiveSensor(numberOfSamples);

      if (rightFiberChargeTime > rightTouchThreshold) {
        rightFiberDoubleTapFlag = true;
        break;
      }
      if (leftFiberChargeTime > leftTouchThreshold) {
        leftSwipeFlag = true;
        break;
      }
    }
  }

  // checking flags
  if (rightSwipeFlag) {
    Serial.println("RIGHT SWIPE DETECTED");
    Serial.println("RIGHT SWIPE DETECTED");
    rightSwipeFlag = false;
  }
  if (leftSwipeFlag) {
    Serial.println("LEFT SWIPE DETECTED");
    Serial.println("LEFT SWIPE DETECTED");
    leftSwipeFlag = false;
  }
  if (rightFiberDoubleTapFlag) {
    Serial.println("RIGHT DOUBLE TAP DETECTED");
    Serial.println("RIGHT DOUBLE TAP DETECTED");
    rightFiberDoubleTapFlag = false;
  }
  if (leftFiberDoubleTapFlag) {
    Serial.println("LEFT DOUBLE TAP DETECTED");
    Serial.println("LEFT DOUBLE TAP DETECTED");
    leftFiberDoubleTapFlag = false;
  }

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
    Serial.print(leftFiber.capacitiveSensor(numberOfSamples));
    Serial.print(" ");
    Serial.println(rightFiber.capacitiveSensor(numberOfSamples));
  }

  for (int counter = 0; counter < 10; counter++) {
    leftMinThreshold += leftFiber.capacitiveSensor(numberOfSamples);
    rightMinThreshold += rightFiber.capacitiveSensor(numberOfSamples);
  }
  leftMinThreshold /= 10;
  rightMinThreshold /= 10;
  Serial.print("LEFT MIN THRESHOLD: ");
  Serial.println(leftMinThreshold);
  Serial.print("RIGHT MIN THRESHOLD: ");
  Serial.println(rightMinThreshold);
  
  rightHoverThreshold = hoverFactor * rightMinThreshold;
  rightTouchThreshold = touchFactor * rightMinThreshold;
  leftHoverThreshold = hoverFactor * leftMinThreshold;
  leftTouchThreshold = touchFactor * leftMinThreshold;
}