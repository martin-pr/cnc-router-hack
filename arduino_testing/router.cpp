#include "router.h"

namespace {
	const float readFloat(const String& s, unsigned char& cursor) {
		// first find the end of the floating point number
		unsigned char end = cursor;
		while(((s[end] >= '0') && (s[end] <= '9')) || (s[end] == '.') || (s[end] == '-'))
			++end;

		char floatbuf[32];
		for(unsigned char a=cursor;a<end;a++)
			floatbuf[a-cursor] = s[a];
		floatbuf[end-cursor] = '\0';

		cursor = end;

		return atof(floatbuf);
	}
}

router::router(const float stepsPerMM[3], motor* motors[3]) : m_delay(1000) {
	for(unsigned char a=0;a<3;a++) {
		m_stepsPerMM[a] = stepsPerMM[a];
		m_motors[a] = motors[a];
	}
}

const unsigned router::delay() const {
	return m_delay;
}

void router::setDelay(unsigned d) {
	m_delay = d;
}

void router::gcode(const String& command) {
	if((command.length() > 4) && (command[0] == 'G') && (command[1] == '0') && ((command[1] == '0') || (command[1] == '1'))) {
		vec<3> target = m_pos;

		unsigned char cursor = 3;
		do {
			// remove all whitespaces at the beginning
			while((command[cursor] == ' ') || (command[cursor] == '\t'))
				++cursor;

			if(command[cursor] != '\0') {
				// x, y and z commands
				if((command[cursor] >= 'X') && (command[cursor] <= 'Z')) {
					const unsigned char axis = command[cursor] - 'X';
					const float val = readFloat(command, ++cursor);
					target[axis] = round(val * m_stepsPerMM[axis]); // !!! round is implemented as a macro!
				}

				// unknown axis
				else {
					Serial << "info ignoring unknown axis " << command[cursor] << " in command " << command << endl;

					// skip it
					while((command[cursor] != ' ') && (command[cursor] != '\t') && (command[cursor] != '\0'))
						++cursor;
				}
			}
		}
		while(command[cursor] != '\0');

		// command fully interpreted, do the movement
		lineTo(target);
	}
}

// Bresenham in 3D - draws a line between m_pos and target (at the end, m_pos == target)
void router::lineTo(const vec<3>& target) {
	// Serial << "info     " << m_pos << "  ->  " << target << endl;

	// the same - nothing to be done
	if(target == m_pos)
		return;

	// compute the deltas and steps + set up the motor directions
	vec<3> delta, step;
	for(unsigned char a=0;a<3;a++) {
		delta[a] = target[a] - m_pos[a];
		if(delta[a] < 0) {
			step[a] = -1;
			m_motors[a]->setDirection(true);
		}
		else {
			step[a] = 1;
			m_motors[a]->setDirection(false);
		}
		delta[a] = abs(delta[a]);
	}

	// figure out the dominant axis
	unsigned char dominant = 0;
	if((delta[1] > delta[0]) && (delta[1] > delta[2]))
		dominant = 1;
	else if((delta[2] > delta[0]) && (delta[2] > delta[1]))
		dominant = 2;

	// the drawing itself
	vec<3> error = delta[dominant] / 2;
	while(m_pos[dominant] != target[dominant]) {
		// step computation
		error = error - delta;
		for(unsigned char a=0;a<3;a++)
			if(error[a] < 0) {
				m_pos[a] += step[a];
				error[a] += delta[dominant];

				// the motor's clock should go "up"
				m_motors[a]->setClock(true);
			}

		// the "plot"
		// Serial << "info      " << m_pos << endl;

		// wait
		delayMicroseconds(m_delay);

		// unset the clock
		for(unsigned char a=0;a<3;a++)
			m_motors[a]->setClock(false);

		// Serial << "info            ";
		// for(unsigned char a=0;a<3;a++)
		// 	Serial << m_motors[a]->value() << " ";
		// Serial << endl;
	}
}
