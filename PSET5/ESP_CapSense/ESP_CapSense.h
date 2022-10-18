/*
 CapacitiveSense.h - Capacitive Sensing Library for 'duino / Wiring
 https://github.com/PaulStoffregen/CapacitiveSensor
 http://www.pjrc.com/teensy/td_libs_CapacitiveSensor.html
 http://playground.arduino.cc/Main/CapacitiveSensor
 Copyright (c) 2009 Paul Bagder
 Updates for other hardare by Paul Stoffregen, 2010-2016
 vim: set ts=4:

 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 DEALINGS IN THE SOFTWARE.
*/

// ensure this library description is only included once
#ifndef ESP_CapSense_h
#define ESP_CapSense_h

#include "Arduino.h"

#define PIN_TO_BASEREG(pin)             (0)
#define PIN_TO_BITMASK(pin)             (pin)
#define IO_REG_TYPE uint32_t

// #define IO_REG_ASM

#define DIRECT_READ(base, pin)          digitalRead(pin)
#define DIRECT_MODE_INPUT(base, pin)    pinMode(pin,INPUT)
#define DIRECT_MODE_OUTPUT(base, pin)   pinMode(pin,OUTPUT)
#define DIRECT_WRITE_LOW(base, pin)     digitalWrite(pin, LOW)
#define DIRECT_WRITE_HIGH(base, pin)    digitalWrite(pin, HIGH)


// library interface description
class CapacitiveSensor
{
  // user-accessible "public" interface
  public:
  // methods
	CapacitiveSensor(uint8_t sendPin, uint8_t receivePin);
	long capacitiveSensorRaw(uint8_t samples);
	long capacitiveSensor(uint8_t samples);
	void set_CS_Timeout_Millis(unsigned long timeout_millis);
	void reset_CS_AutoCal();
	void set_CS_AutocaL_Millis(unsigned long autoCal_millis);
  // library-accessible "private" interface
  private:
  // variables
	int error;
	IO_REG_TYPE  leastTotal;
	IO_REG_TYPE   loopTimingFactor;
	IO_REG_TYPE  CS_Timeout_Millis;
	IO_REG_TYPE  CS_AutocaL_Millis;
	IO_REG_TYPE  lastCal;
	IO_REG_TYPE  total;
	IO_REG_TYPE sBit;   // send pin's ports and bitmask
	volatile IO_REG_TYPE *sReg;
	IO_REG_TYPE rBit;    // receive pin's ports and bitmask
	volatile IO_REG_TYPE *rReg;
  // methods
	int SenseOneCycle(void);
};

#endif
