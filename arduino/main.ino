#include <Arduino.h>
#include <main.cpp>

#include "Display.h"
#include "Buttons.h"

// digital pins
#define X_STEP 2
#define X_DIR 3 // HIGH = negative, LOW = positive
#define Y_STEP 5
#define Y_DIR 6 // HIGH = negative, LOW = positive
#define XY_ENABLE 4

#define Z_ENABLE 7
#define Z_STEP 8
#define Z_DIR 9 // HIGH = negative, LOW = positive

#define PIEZO 10

#define DISP_CLOCK 11
#define DISP_DATA 12
#define DISP_STROBE 13

// analog inputs
#define BUTTONS_0 0
#define BUTTONS_1 1
#define BUTTONS_2 2
#define BUTTONS_3 3

#define X_SENS 4
#define Y_SENS 5
#define Z_SENS 6

#define SPEED 7

// the calibration data
#define X_CENTER 25000
#define Y_CENTER 24000

#define X_LIMIT 50000
#define Y_LIMIT 48000
#define Z_LIMIT 1200

// timing limits
#define MIN_STEP_DELAY 50

////////////////////////////////////////////////////////////////

Display disp(DISP_STROBE, DISP_DATA, DISP_CLOCK);

// position variables
long xPos = X_CENTER, yPos = Y_CENTER, zPos = 0;

///////////////////////////////////////////////////////////////

void initRounter() {
	// initialise the display
	disp.init(2);

	//////////////////////////////////
	// INITIALISATION PROCEDURE
	disp << Display::move_to(0, 0) << "**** INITIALISATION ****";

	// first make sure the limits work
	disp << Display::move_to(0, 1) << "Press the Z limit...";
	while(analogRead(Z_SENS) < 512)
		;
	while(analogRead(Z_SENS) > 512)
		;

	disp << Display::move_to(0, 1) << "Press the X limit...";
	while(analogRead(X_SENS) < 512)
		;
	while(analogRead(X_SENS) > 512)
		;

	disp << Display::move_to(0, 1) << "Press the Y limit...";
	while(analogRead(Y_SENS) < 512)
		;
	while(analogRead(Y_SENS) > 512)
		;


	disp << Display::move_to(0, 1) << "Calibrating Z limit   ";
	digitalWrite(Z_DIR, HIGH);
	while(analogRead(Z_SENS) < 512) {
		digitalWrite(Z_STEP, HIGH);
		delayMicroseconds(200);
		digitalWrite(Z_STEP, LOW);
		delayMicroseconds(200);
	}

	digitalWrite(Z_DIR, LOW);
	while(analogRead(Z_SENS) > 512) {
		digitalWrite(Z_STEP, HIGH);
		delayMicroseconds(50);
		digitalWrite(Z_STEP, LOW);
		delayMicroseconds(50);
	}

	disp << Display::move_to(0, 1) << "Calibrating X limit   ";
	digitalWrite(X_DIR, HIGH);
	while(analogRead(X_SENS) < 512) {
		digitalWrite(X_STEP, HIGH);
		delayMicroseconds(15);
		digitalWrite(X_STEP, LOW);
		delayMicroseconds(15);
	}

	digitalWrite(X_DIR, LOW);
	while(analogRead(X_SENS) < 512) {
		digitalWrite(X_STEP, HIGH);
		delayMicroseconds(15);
		digitalWrite(X_STEP, LOW);
		delayMicroseconds(15);
	}

	disp << Display::move_to(0, 1) << "Calibrating Y limit   ";
	digitalWrite(Y_DIR, HIGH);
	while(analogRead(Y_SENS) < 512) {
		digitalWrite(Y_STEP, HIGH);
		delayMicroseconds(15);
		digitalWrite(Y_STEP, LOW);
		delayMicroseconds(15);
	}

	digitalWrite(Y_DIR, LOW);
	while(analogRead(Y_SENS) > 512) {
		digitalWrite(Y_STEP, HIGH);
		delayMicroseconds(15);
		digitalWrite(Y_STEP, LOW);
		delayMicroseconds(15);
	}

	disp << Display::move_to(0, 1) << "Centering...          ";
	digitalWrite(X_DIR, LOW);
	for(unsigned long a=0;a<X_CENTER;a++) {
		digitalWrite(X_STEP, HIGH);
		delayMicroseconds(50);
		digitalWrite(X_STEP, LOW);
		delayMicroseconds(50);
	}

	digitalWrite(Y_DIR, LOW);
	for(unsigned long a=0;a<Y_CENTER;a++) {
		digitalWrite(Y_STEP, HIGH);
		delayMicroseconds(50);
		digitalWrite(Y_STEP, LOW);
		delayMicroseconds(50);
	}

	disp << Display::move_to(0, 1) << "Ready for action...   ";
}

const String readLine() {
	String result;

	while(!Serial.available())
		;
	char c = Serial.read();

	while(c != '\n') {
		result += c;

		while(!Serial.available())
			;
		c = Serial.read();
	}

	return result;
}

///////////////////////////////////////////////////////////////

void setup() {
	// all pins (apart from the serian inout) set as output
	for(unsigned a=2;a<=13;a++)
		pinMode(a, OUTPUT);

	// initialise the steppers
	digitalWrite(X_STEP, LOW);	// step 0
	digitalWrite(X_DIR, LOW);	// dir 0
	digitalWrite(XY_ENABLE, HIGH);	// enable 0 + 1
	digitalWrite(Y_STEP, LOW);	// step 1
	digitalWrite(Y_DIR, LOW);	// dir 1

	digitalWrite(Z_ENABLE, HIGH);	// enable 2
	digitalWrite(Z_STEP, LOW);	// step 2
	digitalWrite(Z_DIR, LOW);	// dir 2

	// // show the welcome message
	disp.init(2);
	disp << Display::move_to(0, 0) << "(1) USB      (2) power ";
	disp << Display::move_to(0, 1) << "(3) ./router (4) profit";

	// initialise serial port
	Serial.begin(9600);

	// and wait for an "init" command
	String s;
	do {
		if(Serial.available()) {
			s = readLine();

			// Serial.print("'");
			// Serial.print(s);
			// Serial.println("'");
		}
	} while (s != "INIT");


	initRounter();

	Serial.println("OK");
}

static const unsigned char buttonPins[4] = {BUTTONS_0, BUTTONS_1, BUTTONS_2, BUTTONS_3};
Buttons buttons(buttonPins);

// current button state
unsigned long currentButtonState = 0;

// the buffer for commands reading
char commandBuffer[256];

unsigned state = 0;
void loop() {
	/////////////////////
	// MANUAL MOVEMENT

	const unsigned long buttonState = buttons.readState();

	bool xMotion = false;
	bool yMotion = false;
	bool zMotion = false;

	// left
	if((buttonState & (1 << 5)) && (xPos > 0)) {
		digitalWrite(X_DIR, HIGH);
		xMotion = true;
		xPos--;
	}
	// right
	if((buttonState & (1 << 6)) && (xPos < X_LIMIT)) {
		digitalWrite(X_DIR, LOW);
		xMotion = true;
		xPos++;
	}

	// backwards
	if((buttonState & (1 << 4)) && (yPos > 0)) {
		digitalWrite(Y_DIR, HIGH);
		yMotion = true;
		yPos--;
	}
	// forwards
	if((buttonState & (1 << 7)) && (yPos < Y_LIMIT)) {
		digitalWrite(Y_DIR, LOW);
		yMotion = true;
		yPos++;
	}

	// up
	if((buttonState & (1 << 3)) && (zPos > 0)) {
		digitalWrite(Z_DIR, HIGH);
		zMotion = true;
		zPos--;
	}
	// down
	if((buttonState & (1 << 1)) && (zPos < Z_LIMIT)) {
		digitalWrite(Z_DIR, LOW);
		zMotion = true;
		zPos++;
	}

	// do the motion
	if(xMotion || yMotion || zMotion) {
		delayMicroseconds(5);

		if(xMotion)
			digitalWrite(X_STEP, HIGH);

		if(yMotion)
			digitalWrite(Y_STEP, HIGH);

		if(zMotion)
			digitalWrite(Z_STEP, HIGH);

		delayMicroseconds(5);


		digitalWrite(X_STEP, LOW);
		digitalWrite(Y_STEP, LOW);
		digitalWrite(Z_STEP, LOW);
	}


	//////////////////////
	// OTHER BUTTONS

	if(currentButtonState != buttonState) {
		// bbox button (originally menu)
		if(buttonState & (1 << 2))
			Serial.println("bbox");

		// pause
		if(buttonState & (1 << 8))
			Serial.println("pause");

		// run
		if(buttonState & (1 << 10))
			Serial.println("run");

		// home
		if(buttonState & (1 << 11))
			Serial.println("home");

		// stop - immediately stops anything in progress
		if(buttonState & (1 << 15)) {
			Serial.println("stop");
			while(true)
				;
		}

		currentButtonState = buttonState;
	}

	//////////////////////
	// COMMANDS PROCESSING

	// if a command is available on the input
	if(Serial.available()) {
		// read the whole command
		const unsigned charCount = Serial.readBytesUntil('\n', commandBuffer, 256);

		if(charCount > 0) {
			// add the end character (replacing the \n)
			commandBuffer[charCount-1] = '\0';

			// and display it
			disp << Display::move_to(0, 0) << commandBuffer;

			// return a "next" to tell the computer to send another one
			Serial.println("next");
		}
	}


	//////////////////////
	// DISPLAY CONTENT

	// disp.clear();
	// disp << Display::move_to(0,0) << analogRead(SPEED);

	// static unsigned counter = 0;
	// counter++;
	// if(counter == 100) {
	// 	disp.clear();
	// 	disp << Display::move_to(0,0) << "x=" << xPos;
	// 	disp << Display::move_to(8,0) << "y=" << yPos;
	// 	disp << Display::move_to(16,0) << "z=" << zPos;

	// 	disp << Display::move_to(0,1);
	// 	for(int a=15;a>=0;a--)
	// 		if(buttonState & (1 << a))
	// 			disp << '#';
	// 		else
	// 			disp << '-';

	// 	disp << Display::move_to(18,1);
	// 	if(analogRead(X_SENS) > 512)
	// 		disp << "X";
	// 	if(analogRead(Y_SENS) > 512)
	// 		disp << "Y";
	// 	if(analogRead(Z_SENS) > 512)
	// 		disp << "Z";

	// 	disp << " " << analogRead(7);

	// 	counter = 0;
	// }
}
