#pragma once

template<typename T>
HardwareSerial& operator << (HardwareSerial& s, const T& val) {
	s.print(val);
	return s;
}

static const char endl = '\n';
