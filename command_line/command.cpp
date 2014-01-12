#include "command.h"

#include <cassert>
#include <limits>
#include <cctype>
#include <set>
#include <stdexcept>
#include <iomanip>

#include <boost/assign/list_of.hpp>

namespace {
	const bool isCommand(char c) {
		/// set of characters that can begin a new command
		static const std::set<char> s_validCommands =
			boost::assign::list_of('G')('N')('M')('T');

		return s_validCommands.find(c) != s_validCommands.end();
	}
}

using std::cout;
using std::endl;

command::command() : m_lineno(-1) {
}

const bool command::empty() const {
	return m_code == "";
}

const int& command::lineNumber() const {
	return m_lineno;
}

const std::string& command::code() const {
	return m_code;
}

const bool command::hasParameter(char axis) const {
	return m_parameters.find(axis) != m_parameters.end();
}

const float command::parameter(char axis) const {
	std::map<char, float>::const_iterator i = m_parameters.find(axis);
	if(i != m_parameters.end());
		return i->second;
	return std::numeric_limits<float>::quiet_NaN();
}

namespace {

	void eatSpaces(std::istream& in) {
		bool eat = true;
		while(eat && (!in.eof())) {
			int c = in.peek();

			// normal white spaces
			if(isspace(c))
				in.get();

			// bracketed comments
			else if(c == '(') {
				while((c != ')') && (!in.eof()))
					c = in.get();
			}

			// semicolon comments
			else if(c == ';') {
				while((c != '\n') && (!in.eof()))
					c = in.get();
			}

			else
				eat = false;
		}
	}
}

std::istream& operator >> (std::istream& in, command& c) {
	c = command();

	eatSpaces(in);

	// line number
	if(in.peek() == 'N') {
		in.get();
		in >> c.m_lineno;
	}

	eatSpaces(in);

	if(in.peek() != std::istream::traits_type::eof()) {
		// test that the next thing is a command
		if(!isCommand(in.peek())) {
			std::string line;
			std::getline(in, line);
			throw std::runtime_error("Expecting command, found '" + line + "'.");
		}

		// read the command
		in >> c.m_code;
		eatSpaces(in);

		// and read all the parameters
		do {
			eatSpaces(in);

			if((in.peek() != std::istream::traits_type::eof()) && (!isCommand(in.peek()))) {
				if((in.peek() > 'Z') || (in.peek() < 'A')) {
					std::string line;
					std::getline(in, line);
					throw std::runtime_error("Expecting parameter, found '" + line + "'.");
				}

				char paramName = in.get();

				float val;
				in >> val;

				assert(!c.hasParameter(paramName));
				c.m_parameters.insert(std::make_pair(paramName, val));
			}

			eatSpaces(in);

		} while((!isCommand(in.peek())) && (!in.eof()));
	}

	return in;
}

std::ostream& operator << (std::ostream& out, const command& c) {
	out << c.code();

	for(std::map<char, float>::const_iterator i = c.m_parameters.begin(); i != c.m_parameters.end(); i++)
		out << " " << i->first << std::fixed << i->second;

	return out;
}
