#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

#include "document.h"

using std::cout;
using std::endl;

int main(int argc, char* argv[]) {
	if(argc < 2) {
		cout << "Usage: " << argv[0] << " <gcode_file>" << endl;
		return 1;
	}

	// read the input file
	document doc;
	try {
		std::ifstream input(argv[1]);
		input >> doc;
	}

	catch(const std::runtime_error& e) {
		cout << "Error - " << e.what() << endl;
		return 1;
	}

	// print it out for now
	cout << doc << endl;

	// and print the bbox
	cout << "bounding box:" << endl;
	cout << "  x = " << doc.xMin() << " ... " << doc.xMax() << endl;
	cout << "  y = " << doc.yMin() << " ... " << doc.yMax() << endl;
	cout << "  z = " << doc.zMin() << " ... " << doc.zMax() << endl;

	return 0;
}
