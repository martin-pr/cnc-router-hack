#pragma once

#include <Arduino.h>

#include "ShiftRegister.h"

/// a simple HD44780 display encapsulation.
/// Uses shift register-based 3 wire communication and 4-bit version of the communication
/// protocol.
class Display {
	public:
		struct move_to {
			move_to(unsigned char x, unsigned char y);

			unsigned char x, y;
		};

		///////

		/// default constructor, specifying the 3 pins used for shift register.
		/// The connections between the register and the display are:
		///   - 0 = RS
		///   - 1 = R/W
		///   - 2 = CLK
		///   - 3 = unused
		///   - 4-7 = data bit 4-7
		Display(unsigned char strobe, unsigned char data, unsigned char clock);

		/// to be called in setup()
		void init(unsigned char lineCount, bool showCursor = false);

		/// clears the screen
		void clear();

		/// moves the cursor to a position, stdlib style
		Display& operator << (const move_to& m);
		/// prints a String object
		Display& operator << (const String& s);

		/// a templated version of print - prints anything retypable to a String
		template<typename T>
		Display& operator << (const T& val);

	protected:
		/// writes a single character to the LCD
		void writeByte(bool RS, bool RW, unsigned char byte);

	private:
		/// the shift register object used for communication
		ShiftRegister m_register;
};

/////////////

template<typename T>
Display& Display::operator << (const T& val) {
	(*this) << String(val);
	return *this;
}
