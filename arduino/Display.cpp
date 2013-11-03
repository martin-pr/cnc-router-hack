#include "Display.h"

Display::move_to::move_to(unsigned char xIn, unsigned char yIn) : x(xIn), y(yIn) {
}

///////////

Display::Display(unsigned char strobe, unsigned char data, unsigned char clock) : m_register(strobe, data, clock) {
}

void Display::init(unsigned char lineCount, bool showCursor) {
	// set the communication mode first (4 bytes)
	m_register.write(0b00100000);
	m_register.write(0b00100100);
	m_register.write(0b00100000);

	// select the display mode
	writeByte(0, 0, 0b00100000 | (lineCount == 2) << 3);

	// make cursor (in)visible
	writeByte(0, 0, 0b00001100 | showCursor | (showCursor << 1));

	// clear
	clear();

	// select the right cursor mode
	writeByte(0, 0, 0b00010100);
}

void Display::clear() {
	writeByte(0, 0, 0b00000001);
	delay(1);
}

Display& Display::operator << (const move_to& m) {
	writeByte(0, 0, 0b10000000 | (m.x + 0x40*m.y));

	return *this;
}

Display& Display::operator << (const String& s) {
	for(unsigned char a=0;a<s.length();a++)
		writeByte(1, 0, s[a]);
	return *this;
}

void Display::writeByte(bool RS, bool RW, unsigned char byte) {
	unsigned char tmp;

	// upper part (write + clock cycle)
	tmp = (byte & 0b11110000) | RS | (RW << 1);
	m_register.write(tmp);
	m_register.write(tmp | 0b00000100);
	m_register.write(tmp);

	// lower part (write + clock cycle)
	tmp = ((byte & 0b00001111) << 4) | RS | (RW << 1);
	m_register.write(tmp);
	m_register.write(tmp | 0b00000100);
	m_register.write(tmp);
}
