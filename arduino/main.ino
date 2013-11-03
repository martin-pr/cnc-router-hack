#include <Arduino.h>
#include <main.cpp>

#include "Display.h"

#define LEFT_THR 256
#define RIGHT_THR 768

// #define ??_STEP 2
// #define ??_DIR 3
// #define ??_ENABLE 4

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
	int x = analogRead(4);
	int y = analogRead(5);
	int z = analogRead(6);

	if(x < LEFT_THR) {
		digitalWrite(3, HIGH);
		xPos--;
	}
	if(x > RIGHT_THR)  {
		digitalWrite(3, LOW);
		xPos++;
	}

	if(y < LEFT_THR) {
		digitalWrite(6, HIGH);
		yPos--;
	}
	if(y > RIGHT_THR) {
		digitalWrite(6, LOW);
		yPos++;
	}

	if(z < LEFT_THR) {
		digitalWrite(9, HIGH);
		zPos--;
	}
	if(z > RIGHT_THR) {
		digitalWrite(9, LOW);
		zPos++;
	}


	delayMicroseconds(5);


	if((x < LEFT_THR) || (x > RIGHT_THR))
		digitalWrite(2, HIGH);

	if((y < LEFT_THR) || (y > RIGHT_THR))
		digitalWrite(5, HIGH);

	if((z < LEFT_THR) || (z > RIGHT_THR))
		digitalWrite(8, HIGH);


	delayMicroseconds(5);


	digitalWrite(2, LOW);
	digitalWrite(5, LOW);
	digitalWrite(8, LOW);

	static unsigned counter = 0;
	counter++;
	if(counter == 100) {
		disp.clear();
		disp << Display::move_to(0,1) << "x=" << xPos;
		disp << Display::move_to(8,1) << "y=" << yPos;
		disp << Display::move_to(16,1) << "z=" << zPos;

		disp << Display::move_to(0,0) << buttonData(0);
		disp << Display::move_to(6,0) << buttonData(1);
		disp << Display::move_to(12,0) << buttonData(2);
		disp << Display::move_to(18,0) << buttonData(3);
 
		counter = 0;
	}
}
