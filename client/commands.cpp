#include "commands.h"
#include "request.h"

void Commands::registerUser() {
	std::string name;
	std::cout << "Please enter user name: ";
	std::cin >> name;
	std::cout << "Your name: " << name << std::endl;


	std::string public_key;
	Request r = Request();
	r.h.code = 1100;
	std::string payload = name + "\0" + public_key;
	r.h.payload_size = payload.length();
	strncpy(r.p.payload, payload.c_str(), payload.length());
}

void Commands::requestClientsList() {
	Request r = Request();
	r.h.code = 1101;
	r.h.payload_size = 0;
}


void Commands::requestPublicKey() {}
void Commands::requestMessages() {}
void Commands::sendMessage() {}
void Commands::requestSymmetricKey() {}
void Commands::sendSymmetricKey() {}
void Commands::exitProgram() {}
