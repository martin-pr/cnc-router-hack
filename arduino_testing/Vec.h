#pragma once

#include <Arduino.h>

#include "Print.h"

template<unsigned char DIM>
class Vec {
	public:
		Vec() {
			for(unsigned char a=0;a<DIM;a++)
				m_data[a] = 0;
		}

		Vec(const long int data[DIM]) {
			for(unsigned char a=0;a<DIM;a++)
				m_data[a] = data[a];
		}

		Vec(const long int val) {
			for(unsigned char a=0;a<DIM;a++)
				m_data[a] = val;
		}

		const long int& operator[] (unsigned char index) const {
			return m_data[index];
		}

		long int& operator[] (unsigned char index) {
			return m_data[index];
		}

		const Vec operator + (const Vec& v) const {
			Vec res(*this);
			for(unsigned char a=0;a<DIM;a++)
				res[a] += v[a];
			return res;
		}

		const Vec operator - (const Vec& v) const {
			Vec res(*this);
			for(unsigned char a=0;a<DIM;a++)
				res[a] -= v[a];
			return res;
		}

		const bool operator ==(const Vec& v) const {
			for(unsigned char a=0;a<DIM;a++)
				if(m_data[a] != v.m_data[a])
					return false;
			return true;
		}

		const bool operator !=(const Vec& v) const {
			for(unsigned char a=0;a<DIM;a++)
				if(m_data[a] != v.m_data[a])
					return true;
			return false;
		}

		Vec& operator = (const long int val) {
			for(unsigned char a=0;a<DIM;a++)
				m_data[a] = val;
			return *this;
		}

	protected:
	private:
		long int m_data[DIM];
};

template<unsigned char DIM>
HardwareSerial& operator << (HardwareSerial& s, const Vec<DIM>& v) {
	s << v[0];
	for(unsigned char a=1;a<DIM;++a)
		s << " " << v[a];

	return s;
}
