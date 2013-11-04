#include <Arduino.h>
#include <main.cpp>

#include "Display.h"

#define X_STEP 5
#define X_DIR 6
#define Y_STEP 8
#define Y_DIR 9
#define XY_ENABLE 7

#define X_SENS 10
#define Y_SENS 11
#define Z_SENS 12

#define DISP_STROBE 13
#define DISP_DATA 12
#define DISP_CLOCK 11

Display disp(DISP_STROBE, DISP_DATA, DISP_CLOCK);

void setup() {
	// all pins (apart from the serian inout) set as output
	for(unsigned a=2;a<=13;a++)
		pinMode(a, OUTPUT);
	// pinMode(X_SENS, INPUT_PULLUP);
	// pinMode(Y_SENS, INPUT_PULLUP);
	// pinMode(Z_SENS, INPUT_PULLUP);
	// pinMode(13, OUTPUT);


	digitalWrite(2, LOW);	// step 0
	digitalWrite(3, LOW);	// dir 0
	digitalWrite(4, HIGH);	// enable 0

	digitalWrite(5, LOW);	// step 1
	digitalWrite(6, LOW);	// dir 1
	digitalWrite(8, LOW);	// step 2
	digitalWrite(9, LOW);	// dir 2
	digitalWrite(7, HIGH);	// enable 1 + 2

	// initialise the display and print something
	disp.init(2);
	disp << "*** LONDON HACKSPACE ***" << Display::move_to(0, 1) << "Initialising...";

	// initialise serial
	Serial.begin(9600);
	delay(1000);

}

int xPos = 0, yPos = 0, zPos = 0;

unsigned buttonData(unsigned pin) {
	unsigned val = analogRead(pin);
	val += 128;
	val /= 256;
	return val;
}

void loop() {
	if(buttonData(1) == 2) {
		digitalWrite(3, HIGH);
		xPos--;
	}
	if(buttonData(1) == 3)  {
		digitalWrite(3, LOW);
		xPos++;
	}

	if(buttonData(1) == 1) {
		digitalWrite(6, HIGH);
		yPos--;
	}
	if(buttonData(1) == 4) {
		digitalWrite(6, LOW);
		yPos++;
	}

	if(buttonData(0) == 2) {
		digitalWrite(9, HIGH);
		zPos--;
	}
	if(buttonData(0) == 4) {
		digitalWrite(9, LOW);
		zPos++;
	}


	delayMicroseconds(5);


	if((buttonData(1) == 2) || (buttonData(1) == 3))
		digitalWrite(2, HIGH);

	if((buttonData(1) == 1) || (buttonData(1) == 4))
		digitalWrite(5, HIGH);

	if((buttonData(0) == 2) || (buttonData(0) == 4))
		digitalWrite(8, HIGH);


	delayMicroseconds(5);


	digitalWrite(2, LOW);
	digitalWrite(5, LOW);
	digitalWrite(8, LOW);

	static unsigned counter = 0;
	counter++;
	if(counter == 100) {
		disp.clear();
		disp << Display::move_to(0,0) << "x=" << xPos;
		disp << Display::move_to(8,0) << "y=" << yPos;
		disp << Display::move_to(16,0) << "z=" << zPos;

		disp << Display::move_to(0,1) << buttonData(0);
		disp << Display::move_to(6,1) << buttonData(1);
		disp << Display::move_to(12,1) << buttonData(2);
		disp << Display::move_to(18,1) << buttonData(3);

		counter = 0;
	}
}
