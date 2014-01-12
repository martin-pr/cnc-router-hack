#include "document.h"

#include <stdexcept>
#include <sstream>

#include <boost/math/special_functions/fpclassify.hpp>

using std::cout;
using std::endl;

document::document() :
	m_xMin(std::numeric_limits<float>::quiet_NaN()),
	m_xMax(std::numeric_limits<float>::quiet_NaN()),
	m_yMin(std::numeric_limits<float>::quiet_NaN()),
	m_yMax(std::numeric_limits<float>::quiet_NaN()),
	m_zMin(std::numeric_limits<float>::quiet_NaN()),
	m_zMax(std::numeric_limits<float>::quiet_NaN())	{

}

const command& document::operator[](unsigned index) const {

}

const unsigned document::size() const {

}

const float document::xMin() const {
	return m_xMin;
}

const float document::xMax() const {
	return m_xMax;
}

const float document::yMin() const {
	return m_yMin;
}

const float document::yMax() const {
	return m_yMax;
}

const float document::zMin() const {
	return m_zMin;
}

const float document::zMax() const {
	return m_zMax;
}

std::istream& operator >> (std::istream& in, document& doc) {
	bool header = true;
	bool G21 = false, G90 = false;

	while(in.good() && (!in.eof())) {
		command c;
		in >> c;

		// end of reading
		if(c.empty())
			break;

		// transparent commands
		else if((c.code() == "G04") || (c.code()[0] == 'M'))
			doc.m_commands.push_back(c);

		// processed commands
		else {
			// G21 and G90 in the header
			if((c.code() == "G21") && (header)) {
				G21 = true;
				doc.m_commands.push_back(c);
			}
			else if((c.code() == "G90") && (header)) {
				G90 = true;
				doc.m_commands.push_back(c);
			}

			// the rest is G00 and G01 only
			else {
				header = false;
				if((c.code() == "G00") || (c.code() == "G01")) {
					if((!G21) || (!G90))
						throw std::runtime_error("G21 or G90 not present in the file header!");

					if(c.hasParameter('X') && ((boost::math::isnan(doc.m_xMin) || (doc.m_xMin > c.parameter('X')))))
						doc.m_xMin = c.parameter('X');

					if(c.hasParameter('X') && ((boost::math::isnan(doc.m_xMax) || (doc.m_xMax < c.parameter('X')))))
						doc.m_xMax = c.parameter('X');

					if(c.hasParameter('Y') && ((boost::math::isnan(doc.m_yMin) || (doc.m_yMin > c.parameter('Y')))))
						doc.m_yMin = c.parameter('Y');

					if(c.hasParameter('Y') && ((boost::math::isnan(doc.m_yMax) || (doc.m_yMax < c.parameter('Y')))))
						doc.m_yMax = c.parameter('Y');

					if(c.hasParameter('Z') && ((boost::math::isnan(doc.m_zMin) || (doc.m_zMin > c.parameter('Z')))))
						doc.m_zMin = c.parameter('Z');

					if(c.hasParameter('Z') && ((boost::math::isnan(doc.m_zMax) || (doc.m_zMax < c.parameter('Z')))))
						doc.m_zMax = c.parameter('Z');

					doc.m_commands.push_back(c);
				}

				// something not recognized
				else {
					std::stringstream ss;
					ss << "Unknown command '" << c << "'.";
				}
			}
		}

		if(c.empty())
			break;
	}

	return in;
}

std::ostream& operator << (std::ostream& out, const document& doc) {
	for(std::vector<command>::const_iterator i = doc.m_commands.begin(); i != doc.m_commands.end(); i++)
		cout << *i << endl;

	return out;
}
