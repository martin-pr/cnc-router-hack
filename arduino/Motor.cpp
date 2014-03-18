#include "Motor.h"

Motor::Motor(unsigned char stepPin, unsigned char dirPin, long int maxValue, long int val, bool invertDirection) :
	m_direction(false), m_clock(false), m_invertDirection(invertDirection),
	m_stepPin(stepPin), m_dirPin(dirPin),
	m_value(val), m_maxValue(maxValue) {

	digitalWrite(m_stepPin, LOW);
	digitalWrite(m_dirPin, LOW);
}

const bool Motor::direction() const {
	return m_direction;
}

void Motor::setDirection(bool d) {
	m_direction = d;

	if(m_direction == (!m_invertDirection))
		digitalWrite(m_dirPin, HIGH);
	else
		digitalWrite(m_dirPin, LOW);
}

const bool Motor::clock() const {
	return m_clock;
}

void Motor::setClock(bool c) {
	m_clock = false;

	if(c) {
		if(m_direction) {
			if((m_value < m_maxValue) && (m_value >= 0)) {
				digitalWrite(m_stepPin, HIGH);
				m_clock = true;
			}
			else
				digitalWrite(m_stepPin, LOW);

			++m_value;
		}
		else {
			if((m_value > 0) && (m_value <= m_maxValue)) {
				digitalWrite(m_stepPin, HIGH);
				m_clock = true;
			}
			else
				digitalWrite(m_stepPin, LOW);

			--m_value;
		}
	}

	else
		digitalWrite(m_stepPin, LOW);
	// digitalWrite(m_stepPin, c);
}

const long int& Motor::value() const {
	return m_value;
}
