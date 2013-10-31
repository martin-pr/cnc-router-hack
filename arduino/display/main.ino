#include <Arduino.h>
#include <main.cpp>

#include "Display.h"

#define STROBE 13
#define DATA 12
#define CLOCK 11

Display disp(STROBE, DATA, CLOCK);

void setup() {
	// all pins (apart from the serian inout) set as output
	for(unsigned a=2;a<=13;a++) {
		pinMode(a, OUTPUT);
		digitalWrite(a, LOW);
	}

	// init the display for two lines
	disp.init(2);

	// print something
	disp << "Hi Giulia! " << Display::move_to(0, 1) << "How are you?";
}

void loop() {
}
