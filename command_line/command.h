#pragma once

#include <iostream>
#include <string>
#include <map>

class command {
	public:
		command();

		const bool empty() const;

		const int& lineNumber() const; // -1 = none
		const std::string& code() const;

		const bool hasParameter(char axis) const;
		const float parameter(char axis) const;

	protected:
	private:
		int m_lineno;
		std::string m_code;
		std::map<char, float> m_parameters;

	friend std::istream& operator >> (std::istream& in, command& c);
	friend std::ostream& operator << (std::ostream& out, const command& c);
};

std::istream& operator >> (std::istream& in, command& c);
std::ostream& operator << (std::ostream& out, const command& c);
