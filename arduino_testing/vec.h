#pragma once

#include <Arduino.h>

#include "print.h"

template<unsigned char DIM>
class vec {
	public:
		vec() {
			for(unsigned char a=0;a<DIM;a++)
				m_data[a] = 0;
		}

		vec(const long int data[DIM]) {
			for(unsigned char a=0;a<DIM;a++)
				m_data[a] = data[a];
		}

		vec(const long int val) {
			for(unsigned char a=0;a<DIM;a++)
				m_data[a] = val;
		}

		const long int& operator[] (unsigned char index) const {
			return m_data[index];
		}

		long int& operator[] (unsigned char index) {
			return m_data[index];
		}

		const vec operator + (const vec& v) const {
			vec res(*this);
			for(unsigned char a=0;a<DIM;a++)
				res[a] += v[a];
			return res;
		}

		const vec operator - (const vec& v) const {
			vec res(*this);
			for(unsigned char a=0;a<DIM;a++)
				res[a] -= v[a];
			return res;
		}

		const bool operator ==(const vec& v) const {
			for(unsigned char a=0;a<DIM;a++)
				if(m_data[a] != v.m_data[a])
					return false;
			return true;
		}

		const bool operator !=(const vec& v) const {
			for(unsigned char a=0;a<DIM;a++)
				if(m_data[a] != v.m_data[a])
					return true;
			return false;
		}

		vec& operator = (const long int val) {
			for(unsigned char a=0;a<DIM;a++)
				m_data[a] = val;
			return *this;
		}

	protected:
	private:
		long int m_data[DIM];
};

template<unsigned char DIM>
HardwareSerial& operator << (HardwareSerial& s, const vec<DIM>& v) {
	s << v[0];
	for(unsigned char a=1;a<DIM;++a)
		s << " " << v[a];

	return s;
}
