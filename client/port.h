#pragma once
#include <string>
#include <fstream>
#include <iostream>

#define PORT_INFO_FILENAME "port.info"

class Port 
{
	int port;

	bool is_valid_port(std::string port) {
		if (is_number(port)) {
			return true;
		}
		return false;
	}

	bool is_number(const std::string& s) {
		return !s.empty() && std::find_if(s.begin(),
			s.end(), [](unsigned char c) { return !isdigit(c); }) == s.end();
	}
public:
	Port();

	int getPort();
};