#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

#include "document.h"
#include "command.h"

using std::cout;
using std::endl;

int main(int argc, char* argv[]) {
	if(argc < 2) {
		cout << "Usage: " << argv[0] << " <gcode_file>" << endl;
		return 1;
	}

	// read the input file
	try {
		std::ifstream input(argv[1]);
		while(input.good() && (!input.eof())) {
			command c;
			input >> c;

			cout << c << endl;

			if(c.empty())
				break;
		}
	}

	catch(const std::runtime_error& e) {
		cout << "Error - " << e.what() << endl;
		return 1;
	}

	return 0;
}
