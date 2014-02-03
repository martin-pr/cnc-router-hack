#include "interface.h"

#include <boost/bind.hpp>

#include <readline/readline.h>
#include <readline/history.h>

using std::cout;
using std::endl;

namespace {
	const std::string currentDir() {
		// get the current directory
		char buffer[256];
		const unsigned cnt = readlink("/proc/self/exe", buffer, 256);
		assert(cnt < 255);
		buffer[cnt] = '\0';

		const std::string dir = buffer;
		assert(dir.rfind('/') != std::string::npos);

		return dir.substr(0, dir.rfind('/'));
	}
}

interface::interface() {
	addFunction("help", "prints this message", boost::bind(&interface::help, this, _1));
	addFunction("exit", "exits the interface", boost::bind(&interface::exit, this, _1));

	read_history((currentDir() + "/.history").c_str());
}

interface::~interface() {
	write_history((currentDir() + "/.history").c_str());
}

void interface::addFunction(const std::string& key, const std::string& desc, const boost::function< void(std::stringstream&) >& fn) {
	m_descriptions << key << "\t" << desc << endl;

	m_functions.insert(std::make_pair(key, fn));
}

void interface::exec() {
	while(true) {
		char* line = readline("> ");
		if(!line)
			break;
		
		if(*line) {
			add_history(line);
			std::stringstream lineStr(line);
			std::free(line);

			std::string command;
			lineStr >> command;

			if(command == "exit")
				break;
			else {
				std::map<std::string, boost::function< void(std::stringstream&) > >::const_iterator i = m_functions.find(command);
				if(i == m_functions.end())
					cout << "Unknown command '" << command << "'." << endl;
				else
					i->second(lineStr);
			}
		}
	}
}

void interface::help(std::stringstream& ss) {
	cout << m_descriptions.str();
}

void interface::exit(std::stringstream& ss) {
	// dummy
}