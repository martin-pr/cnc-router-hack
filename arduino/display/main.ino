#include <Arduino.h>
#include <main.cpp>

#include "ShiftRegister.h"

void setup() {
	// all pins (apart from the serian inout) set as output
	for(unsigned a=2;a<=13;a++) {
		pinMode(a, OUTPUT);
		digitalWrite(a, LOW);
	}
}

/////////////////

#define STROBE 13
#define DATA 12
#define CLOCK 11

ShiftRegister reg(STROBE, DATA, CLOCK);

void loop() {
	for(unsigned a=0;a<8;a++) {
		reg.write(1 << a);
		// reg.write(0);
		delay(100);
	}
}
