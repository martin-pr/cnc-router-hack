#pragma once

/// a simple shift register object, encapsulating writing a single byte to the register and
///   sending it to its output
class ShiftRegister {
	public:
		/// creates the register object, spacifying the 3 connection pins
		ShiftRegister(unsigned char strobe, unsigned char data, unsigned char clock);

		/// writes a byte into the register and flips strobe signal to output the result
		void write(unsigned char value);

	protected:
	private:
		unsigned char m_strobe, m_data, m_clock;
};
