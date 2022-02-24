#include <iostream>
#include <string>
#include "server.h"
#include "connection.h"
#include "protocol.h"
#include "menu.h"

int main() {
	// fetch server's host & port
	server s = server();

	std::string host = s.getHost();
	int port = s.getPort();
	
	// connect to server
	connection* c = new connection(host, std::to_string(port));

	// init protocol
	protocol protocol(c);

	// init menu
	Menu menu = Menu();

	// execute user requests
	int command_code = 1;
	while (command_code) {
		menu.show_menu();
		std::cout << "? ";
		std::cin >> command_code;
		protocol.handle(command_code);
	}

	return 0;
}