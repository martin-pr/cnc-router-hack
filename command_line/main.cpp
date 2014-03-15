#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <unistd.h>
#include <cassert>

#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

#include <SerialPort.h>

#include "document.h"

using std::cout;
using std::endl;
using std::cerr;

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

	SerialPort ser("/dev/ttyUSB0");
	ser.Open(SerialPort::BAUD_9600, SerialPort::CHAR_SIZE_8, SerialPort::PARITY_NONE, SerialPort::STOP_BITS_1, SerialPort::FLOW_CONTROL_NONE);

	sleep(2);
	if(!ser.IsOpen()) {
		cout << "Error opening serial port." << endl;
		return 1;
	}

	std::string line;

	// first send an "init" command, to initialise the router
	cout << "Sending init command... " << std::flush;
	ser.Write("INIT\n");

	do {
		line = ser.ReadLine();
		boost::algorithm::trim(line);

		if(line != "OK")
			cout << "error: " << line << endl;

	} while(line != "OK");

	cout << "done." << endl;

	cout << "Now please use the buttons on the router to do stuff... (stop buttons immediately exits)" << endl;

	//////////////////////////////////////////
	// the main command loop

	// true when the router is running
	bool running = false;
	// true when a next command can be send
	bool nextCommand = true;
	// the index of the command to be send
	unsigned commandCounter = 0;

	while(ser.IsOpen()) {
		line = ser.ReadLine();
		boost::algorithm::trim(line);

		if(line == "stop") {
			cout << "'stop' command, exiting." << endl;
			return 0;
		}

		else if(line == "run") {
			cout << "'stop' command, routing..." << endl;
			running = true;
		}

		else if(line == "pause") {
			cout << "'pause' command, waiting..." << endl;
			running = false;
		}

		else if(line == "next")
			nextCommand = true;

		// send a new command
		if(nextCommand && running) {
			if(commandCounter < doc.size()) {
				std::stringstream com;
				com << doc[commandCounter] << endl;
				ser.Write(com.str());

				cout << "sending command " << commandCounter << "/" << (doc.size()-1) << endl;

				++commandCounter;
				nextCommand = false;
			}
		}


	}

	ser.Close();

	return 0;
}
