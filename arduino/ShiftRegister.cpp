#include "ShiftRegister.h"

#include <Arduino.h>

ShiftRegister::ShiftRegister(unsigned char strobe, unsigned char data, unsigned char clock) :
	m_strobe(strobe), m_data(data), m_clock(clock) {
}

void ShiftRegister::write(unsigned char value) {
	// make sure the strobe is down
	digitalWrite(m_strobe, LOW);

	// write the data
	for(unsigned char a=0;a<8;a++) {
		// write (from highest order bit down), and shift the value
		digitalWrite(m_data, value & 0b10000000);
		value = value << 1;

		// clock cycle
		digitalWrite(m_clock, LOW);
		digitalWrite(m_clock, HIGH);
	}

	// flip the strobe to write to the output
	digitalWrite(m_strobe, HIGH);
	digitalWrite(m_strobe, LOW);
}
