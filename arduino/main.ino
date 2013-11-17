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

static const unsigned char buttonPins[4] = {BUTTONS_0, BUTTONS_1, BUTTONS_2, BUTTONS_3};
Buttons buttons(buttonPins);

unsigned state = 0;
void loop() {
	const unsigned long buttonState = buttons.readState();

	if(buttonState & (1 << 5)) {
		digitalWrite(X_DIR, HIGH);
		xPos--;
	}
	if(buttonState & (1 << 6)) {
		digitalWrite(X_DIR, LOW);
		xPos++;
	}

	if(buttonState & (1 << 4)) {
		digitalWrite(Y_DIR, HIGH);
		yPos--;
	}
	if(buttonState & (1 << 7)) {
		digitalWrite(Y_DIR, LOW);
		yPos++;
	}

	if(buttonState & (1 << 1)) {
		digitalWrite(Z_DIR, HIGH);
		zPos--;
	}
	if(buttonState & (1 << 3)) {
		digitalWrite(X_DIR, LOW);
		zPos++;
	}

	delayMicroseconds(5);


	if((buttonState & (1 << 5)) || (buttonState & (1 << 6)))
		digitalWrite(X_STEP, HIGH);

	if((buttonState & (1 << 4)) || (buttonState & (1 << 7)))
		digitalWrite(Y_STEP, HIGH);

	if((buttonState & (1 << 1)) || (buttonState & (1 << 3)))
		digitalWrite(Z_STEP, HIGH);


	delayMicroseconds(5);


	digitalWrite(X_STEP, LOW);
	digitalWrite(Y_STEP, LOW);
	digitalWrite(Z_STEP, LOW);

	static unsigned counter = 0;
	counter++;
	if(counter == 100) {
		disp.clear();
		disp << Display::move_to(0,0) << "x=" << xPos;
		disp << Display::move_to(8,0) << "y=" << yPos;
		disp << Display::move_to(16,0) << "z=" << zPos;

		disp << Display::move_to(0,1);
		for(int a=15;a>=0;a--)
			if(buttonState & (1 << a))
				disp << '#';
			else
				disp << '-';

		disp << Display::move_to(18,1);
		if(analogRead(X_SENS) > 512)
			disp << "X";
		if(analogRead(Y_SENS) > 512)
			disp << "Y";
		if(analogRead(Z_SENS) > 512)
			disp << "Z";

		disp << " " << analogRead(7);

		counter = 0;
	}
}
