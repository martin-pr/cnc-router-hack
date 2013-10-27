#include <Arduino.h>
#include <main.cpp>

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

void setup() {
	// all pins (apart from the serian inout) set as output
	for(unsigned a=2;a<=13;a++)
		pinMode(a, OUTPUT);
	pinMode(X_SENS, INPUT_PULLUP);
	pinMode(Y_SENS, INPUT_PULLUP);
	pinMode(Z_SENS, INPUT_PULLUP);
	pinMode(13, OUTPUT);

	// initialise serial
	Serial.begin(9600);

	// broken
	digitalWrite(2, LOW);	// step 0
	digitalWrite(3, LOW);	// dir 0
	digitalWrite(4, HIGH);	// enable 0

	digitalWrite(5, LOW);	// step 1
	digitalWrite(6, LOW);	// dir 1
	digitalWrite(8, LOW);	// step 2
	digitalWrite(9, LOW);	// dir 2
	digitalWrite(7, HIGH);	// enable 1 + 2

	digitalWrite(13, LOW);
	// 10, 11, 12 - sensor inputs

}

int xPos = 0, yPos = 0, zPos = 0;

void loop() {

	int x = analogRead(0);
	int y = analogRead(1);
	int z = analogRead(2);

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
		Serial.print("x=");
		Serial.print(xPos);
		Serial.print(" (");
		Serial.print(digitalRead(10));
		Serial.print(")  ");
		
		Serial.print("y=");
		Serial.print(yPos);
		Serial.print(" (");
		Serial.print(digitalRead(11));
		Serial.print(")  ");
		
		Serial.print("z=");
		Serial.print(zPos);
		Serial.print(" (");
		Serial.print(digitalRead(12));
		Serial.print(")  ");

		Serial.println();

		counter = 0;
	}
}
