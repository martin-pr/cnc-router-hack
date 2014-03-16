#pragma once

#include <Arduino.h>

class Motor {
	public:
		Motor(unsigned char stepPin, unsigned char dirPin, long int maxValue, bool invertDirection = false);

		const bool direction() const;
		void setDirection(bool d);

		const bool clock() const;
		void setClock(bool c);

		const long int& value() const;

	protected:
	private:
		bool m_direction, m_clock, m_invertDirection;

		unsigned char m_stepPin, m_dirPin;
		long int m_value, m_maxValue;
};
