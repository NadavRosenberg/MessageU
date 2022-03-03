#include <iostream>
#include <string>
#include "server.h"
#include "connection.h"
#include "ProtocolWrapper.h"
#include "menu.h"
#include "test.h"

int main() {
	//sendMessage();
	//shareSymmetric();
	//exit_program();


	// fetch server's host & port
	server s = server();

	std::string host = s.getHost();
	int port = s.getPort();
	
	// connect to server
	connection* c = new connection(host, std::to_string(port));

	// init profile
	profile* prof = new profile();
	prof->fetchData();

	// init users
	users usr;

	// init protocol
	protocol prcl(c, prof, &usr);

	// init protocol's wrapper
	ProtocolWrapper prwr(&prcl, &usr);

	// init menu
	Menu menu = Menu();

	// execute user requests
	int command_code = 1;
	while (command_code) {
		menu.show_menu();
		std::cout << "? ";
		std::cin >> command_code;
		prwr.handle(command_code);
	}

	return 0;
}