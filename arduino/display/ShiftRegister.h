#pragma once

class ShiftRegister {
	public:
		ShiftRegister(unsigned char strobe, unsigned char data, unsigned char clock);
		void write(unsigned char value);

	protected:
	private:
		unsigned char m_strobe, m_data, m_clock;
};
