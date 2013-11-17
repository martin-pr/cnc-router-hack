#pragma once

/// trivially filtered buttons class.
/// Returns the buttons value (1 on nth bit ~ 16 buttons max) only after the button
/// value didn't change for a number of cycles.
class Buttons {
	public:
		Buttons(const unsigned char pins[4], const unsigned char measureCount = 5);

		const unsigned long readState();

	protected:
	private:
		unsigned char m_pins[4];
		unsigned char m_measureCount;

		unsigned long m_currentState;
		unsigned m_counter;
};
