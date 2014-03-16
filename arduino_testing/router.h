#pragma once

#include <Arduino.h>

#include "vec.h"
#include "motor.h"

class router {
	public:
		router(const float stepsPerMM[3], motor* motors[3]);

		const unsigned delay() const;
		void setDelay(unsigned d);

		void gcode(const String& command);

	protected:
		// linear interpolation using Bresenham's algorithm
		void lineTo(const vec<3>& target);

	private:
		vec<3> m_pos;
		float m_stepsPerMM[3];
		motor* m_motors[3];

		unsigned m_delay;
};
