#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include "utils.h"

#define SERVER_FILENAME "server.info"
#define MAX_PORT 65535
#define MIN_PORT 0

class server {
private:
	std::string host;
	int port;
	bool is_valid_host(std::string s);
	bool is_valid_port(std::string s);
public:
	server();
	int getPort();
	std::string getHost();
};
