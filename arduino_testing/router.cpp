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

router::router(const float stepsPerMM[3]) {
	for(unsigned char a=0;a<3;a++)
		m_stepsPerMM[a] = stepsPerMM[a];
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
					Serial.print("info ignoring unknown axis ");
					Serial.print(command[cursor]);
					Serial.print(" in command ");
					Serial.println(command);

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

void router::lineTo(const vec<3>& target) {
	Serial.print("info     ");
	Serial.print(m_pos[0]);
	Serial.print(" ");
	Serial.print(m_pos[1]);
	Serial.print(" ");
	Serial.print(m_pos[2]);
	Serial.print(" -> ");
	Serial.print(target[0]);
	Serial.print(" ");
	Serial.print(target[1]);
	Serial.print(" ");
	Serial.println(target[2]);

	m_pos = target;
}
