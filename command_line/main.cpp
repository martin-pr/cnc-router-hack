#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <unistd.h>
#include <cassert>

#include <boost/bind.hpp>

#include "document.h"
#include "interface.h"

using std::cout;
using std::endl;

void bbox(const document& doc, std::stringstream& ss) {
	cout << "bounding box:" << endl;
	cout << "  x = " << doc.xMin() << "mm ... " << doc.xMax() << "mm" << endl;
	cout << "  y = " << doc.yMin() << "mm ... " << doc.yMax() << "mm" << endl;
	cout << "  z = " << doc.zMin() << "mm ... " << doc.zMax() << "mm" << endl;
}

void print(const document& doc, std::stringstream& ss) {
	cout << doc << endl;
}

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

	interface i;

	i.addFunction("bbox", "prints the bounding box", boost::bind(bbox, boost::cref(doc), _1));
	i.addFunction("print", "prints the entire processed input file to the console output", boost::bind(print, boost::cref(doc), _1));

	i.exec();

	return 0;
}