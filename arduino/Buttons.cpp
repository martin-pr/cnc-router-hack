#include "Buttons.h"

#include <Arduino.h>

#define MEASURE_COUNT 5

namespace {

/// reads a button's analog value, and returns 0-4 (0 = no buttons, 1-4 = button id)
unsigned char buttonData(unsigned pin) {
	unsigned val = analogRead(pin);
	val += 128;
	val /= 256;
	return val;
}

}

Buttons::Buttons(const unsigned char pins[4], const unsigned char measures) :
	m_measureCount(measures), m_currentState(0), m_counter(0) {

	for(unsigned char a=0;a<4;a++)
		m_pins[a] = pins[a];
}

const unsigned long Buttons::readState() {
	// assemble the current value
	unsigned long current = 0;
	for(unsigned char pin = 0; pin < 4; ++pin) {
		unsigned char val = buttonData(pin);
		if(val > 0)
			current = current | (1 << ((val-1) + pin*4));
	}

	// filter - test if it is the same as the previous one, otherwise reset the counter
	if(m_currentState == current)
		m_counter = min(m_counter+1, MEASURE_COUNT);
	else {
		m_currentState = current;
		m_counter = 0;
	}

	// return 0 if the measure count is too low, otherwise return the state
	if(m_counter < MEASURE_COUNT)
		return 0;
	return m_currentState;
}
