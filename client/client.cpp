#include <iostream>
#include <string>
#include "server.h"
#include "connection.h"
#include "protocol.h"
#include "menu.h"
#include "profile.h"

int VERSION = 1;

int main() {
	// fetch server's host & port
	server s = server();

	std::string host = s.getHost();
	int port = s.getPort();
	
	// connect to server
	connection c(host, std::to_string(port));
	c.connect();

	// init protocol
	protocol protocol(c);

	// init menu
	Menu menu = Menu();

	// execute user requests
	int req_code = 1;
	while (req_code) {
		menu.show_menu();
		std::cin >> req_code;
		protocol.handle(req_code);
	}

	return 0;

	//Profile profile = Profile();

}