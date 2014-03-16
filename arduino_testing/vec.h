#pragma once

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

		const long int& operator[] (unsigned char index) const {
			return m_data[index];
		}

		long int& operator[] (unsigned char index) {
			return m_data[index];
		}

		const vec operator + (const vec& v) {
			vec res(*this);
			for(unsigned char a=0;a<DIM;a++)
				res[a] += v[a];
		}

		const vec operator - (const vec& v) {
			vec res(*this);
			for(unsigned char a=0;a<DIM;a++)
				res[a] -= v[a];
		}

	protected:
	private:
		long int m_data[DIM];
};

