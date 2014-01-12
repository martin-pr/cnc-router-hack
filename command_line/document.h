#pragma once

#include <vector>

#include "command.h"

/// document class, hardcodes assumptions about processable documents.
/// Removes all comments (both bracketted and semicolon-separated).
/// Expects mm (G21) and absolute (G90) commands before any movement;
/// transparent to M* and G04 commands;
/// supports only G00 and G01 movement commands.
class document {
	public:
		document();

		const command& operator[](unsigned index) const;
		const unsigned size() const;

		// bbox access methods
		const float xMin() const;
		const float xMax() const;
		const float yMin() const;
		const float yMax() const;
		const float zMin() const;
		const float zMax() const;

	protected:
	private:
		float m_xMin, m_xMax, m_yMin, m_yMax, m_zMin, m_zMax;

		std::vector<command> m_commands;

	friend std::istream& operator >> (std::istream& in, document& doc);
	friend std::ostream& operator << (std::ostream& out, const document& doc);
};

std::istream& operator >> (std::istream& in, document& doc);
std::ostream& operator << (std::ostream& out, const document& doc);
