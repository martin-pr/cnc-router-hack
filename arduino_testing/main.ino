#include <Arduino.h>
#include <main.cpp>

#include "Router.h"
#include "Motor.h"
#include "Print.h"

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

//////////////////////////

Motor XMotor(2, 3, 50000);
Motor YMotor(5, 6, 48000);
Motor ZMotor(8, 9, 1200);

float divisors[3] = {100.0f, 100.0f, 100.0f};
Motor* motors[3] = {&XMotor, &YMotor, &ZMotor};
Router router(divisors, motors);

void setup() {
	// initialise serial port
	Serial.begin(9600);

	// and wait for an "init" command
	String s;
	do {
		if(Serial.available())
			s = readLine();
	} while (s != "INIT");

	Serial << "OK" << endl;

	delay(1000);
	Serial << "run" << endl;
}

void loop() {
	if(Serial.available()) {
		String s = readLine();

		// ASSUMES THAT EACH COMMAND IS ON ITS OWN LINE
		// G00 and G01 are interpreted in the same way
		if((s.length() > 4) && (s[0] == 'G') && (s[1] == '0') && ((s[2] == '0') || (s[2] == '1'))) {
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
