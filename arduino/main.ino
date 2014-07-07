#include <Arduino.h>
#include <main.cpp>

#include "Display.h"
#include "Buttons.h"
#include "Print.h"
#include "Motor.h"
#include "Router.h"

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
#define Z_CENTER 1200

#define X_LIMIT 50000
#define Y_LIMIT 48000
#define Z_LIMIT 1200

// timing limits
#define MIN_STEP_DELAY 50

////////////////////////////////////////////////////////////////

Display disp(DISP_STROBE, DISP_DATA, DISP_CLOCK);

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

	disp << Display::move_to(0, 0) << "*  LONDON  HACKSPACE  *";
	disp << Display::move_to(0, 1) << "*    router ready     *";

	Serial << "OK" << endl;
}

static const unsigned char buttonPins[4] = {BUTTONS_0, BUTTONS_1, BUTTONS_2, BUTTONS_3};
Buttons buttons(buttonPins);

// current button state
unsigned long currentButtonState = 0;

// the buffer for commands reading
char commandBuffer[256];

// the motors
Motor XMotor(X_STEP, X_DIR, X_LIMIT, X_CENTER);
Motor YMotor(Y_STEP, Y_DIR, Y_LIMIT, Y_CENTER);
Motor ZMotor(Z_STEP, Z_DIR, Z_LIMIT, Z_CENTER);

// divisors - measured on the physical machine
//   # of steps per mm
float divisors[3] = {155.5f, 155.5f, -92.31f};
Motor* motors[3] = {&XMotor, &YMotor, &ZMotor};
Router router(divisors, motors);

unsigned state = 0;
void loop() {
	/////////////////////
	// MANUAL MOVEMENT

	const unsigned long buttonState = buttons.readState();

	// left
	if(buttonState & (1 << 5)) {
		XMotor.setDirection(true);
		XMotor.setClock(true);
	}
	// right
	else if(buttonState & (1 << 6)) {
		XMotor.setDirection(false);
		XMotor.setClock(true);
	}

	// backwards
	if(buttonState & (1 << 4)) {
		YMotor.setDirection(true);
		YMotor.setClock(true);
	}
	// forwards
	else if(buttonState & (1 << 7)) {
		YMotor.setDirection(false);
		YMotor.setClock(true);
	}

	// up
	if(buttonState & (1 << 3)) {
		ZMotor.setDirection(true);
		ZMotor.setClock(true);
	}
	// down
	else if(buttonState & (1 << 1)) {
		ZMotor.setDirection(false);
		ZMotor.setClock(true);
	}

	// do the motion
	if(XMotor.clock() || YMotor.clock() || ZMotor.clock()) {
		delayMicroseconds(10);

		XMotor.setClock(false);
		YMotor.setClock(false);
		ZMotor.setClock(false);
	}


	//////////////////////
	// OTHER BUTTONS

	if(currentButtonState != buttonState) {
		// bbox button (originally menu)
		if(buttonState & (1 << 2))
			Serial << "bbox" << endl;

		// pause
		if(buttonState & (1 << 8))
			Serial << "pause" << endl;

		// run
		if(buttonState & (1 << 10))
			Serial << "run" << endl;

		// home
		if(buttonState & (1 << 11))
			Serial << "home" << endl;

		// stop - immediately stops anything in progress
		if(buttonState & (1 << 15)) {
			Serial << "stop" << endl;
			while(true)
				;
		}

		currentButtonState = buttonState;
	}

	//////////////////////
	// COMMANDS PROCESSING

	// // if a command is available on the input
	// if(Serial.available()) {
	// 	// read the whole command
	// 	const unsigned charCount = Serial.readBytesUntil('\n', commandBuffer, 256);

	// 	if(charCount > 0) {
	// 		// add the end character (replacing the \n)
	// 		commandBuffer[charCount-1] = '\0';

	// 		// and display it
	// 		disp << Display::move_to(0, 1) << commandBuffer;

	// 		// return a "next" to tell the computer to send another one
	// 		Serial << "next" << endl;
	// 	}
	// }

	if(Serial.available()) {
		// read the next line from input
		String s = readLine();

		// ASSUMES THAT EACH COMMAND IS ON ITS OWN LINE
		// G00 and G01 are interpreted in the same way, except for speed
		if((s.length() > 4) && (s[0] == 'G') && (s[1] == '0') && ((s[2] == '0') || (s[2] == '1'))) {
			// set the speed - max for G00, acc to the pot for G01
			if(s[2] == '0')
				// G00 = maximum speed movement
				router.setDelay(1000);
			else
				// read the speed and set it as the delay of the router
				//   - values determine the delay between two steps in microseconds, with 1000 as minimum and 10000 as maximum
				router.setDelay(1000 + 9 * (1023 - analogRead(SPEED)));

			// blink
			digitalWrite(13, HIGH);
			delay(1);
			digitalWrite(13, LOW);

			// Serial << "info " << s << endl;

			router.gcode(s);
		}

		// G20 means the g-code file was in imperial units - just fail
		else if((s.length() > 4) && (s[0] == 'G') && (s[1] == '2') && (s[2] == '0'))
			Serial.print("error - g-code is in imperial units, this router works only in mm");

		// G22 means the g-code file was in mm - all good
		else if((s.length() > 4) && (s[0] == 'G') && (s[1] == '2') && (s[2] == '1'))
			;

		// an unknown command - ignore
		else {
			Serial.print("info ignored command '");
			Serial.print(s);
			Serial.print("'\n");
		}

		Serial.print("next\n");
	}
}
