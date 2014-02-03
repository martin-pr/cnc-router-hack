#pragma once

#include <string>
#include <sstream>
#include <map>

#include <boost/function.hpp>

class interface {
	public:
		interface();
		~interface();

		void addFunction(const std::string& key, const std::string& desc, const boost::function< void(std::stringstream&) >& fn);

		void exec();

	protected:
		void help(std::stringstream& ss);
		void exit(std::stringstream& ss);

	private:
		std::map<std::string, boost::function< void(std::stringstream&) > > m_functions;

		std::stringstream m_descriptions;
};