#pragma once

#include <Arduino.h>

#include "Vec.h"
#include "Motor.h"

class Router {
	public:
		Router(const float stepsPerMM[3], Motor* motors[3]);

		const unsigned delay() const;
		void setDelay(unsigned d);

		void gcode(const String& command);

	protected:
		// linear interpolation using Bresenham's algorithm
		void lineTo(const Vec<3>& target);

	private:
		Vec<3> m_pos;
		float m_stepsPerMM[3];
		Motor* m_motors[3];

		unsigned m_delay;
};
