#include "motor.h"

motor::motor(unsigned char stepPin, unsigned char dirPin, long int maxValue, bool invertDirection) :
	m_direction(false), m_clock(false), m_invertDirection(invertDirection),
	m_stepPin(stepPin), m_dirPin(dirPin),
	m_value(0), m_maxValue(maxValue) {

}

const bool motor::direction() const {
	return m_direction;
}

void motor::setDirection(bool d) {
	m_direction = d;

	if(m_direction == (!m_invertDirection))
		digitalWrite(m_dirPin, HIGH);
	else
		digitalWrite(m_dirPin, LOW);
}

const bool motor::clock() const {
	return m_clock;
}

void motor::setClock(bool c) {
	if(c) {
		if(m_direction == (m_invertDirection)) {
			if(m_value + 1 <= m_maxValue)
				digitalWrite(m_stepPin, HIGH);
			else
				digitalWrite(m_stepPin, LOW);

			++m_value;
		}
		else {
			if(m_value - 1 >= 0)
				digitalWrite(m_stepPin, HIGH);
			else
				digitalWrite(m_stepPin, LOW);

			--m_value;
		}
	}

	else
		digitalWrite(m_stepPin, LOW);
}

const long int& motor::value() const {
	return m_value;
}
