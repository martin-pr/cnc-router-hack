#pragma once

#include <Arduino.h>

#include "vec.h"

class router {
	public:
		router(const float stepsPerMM[3]);

		void gcode(const String& command);

	protected:
		void lineTo(const vec<3>& target);

	private:
		vec<3> m_pos;
		float m_stepsPerMM[3];
};
