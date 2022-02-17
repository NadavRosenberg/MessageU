#include "protocol.h"

protocol::protocol(connection c): conn(c) {
}

void protocol::handle(int req_code) {
	switch (req_code) {
	case 110:
		registerUser();
		break;
	case 120:
		requestClientsList();
		break;
	case 130:
		requestPublicKey();
		break;
	case 140:
		requestMessages();
		break;
	case 150:
		sendMessage();
		break;
	case 151:
		requestSymmetricKey();
		break;
	case 152:
		sendSymmetricKey();
		break;
	default:
		std::cout << "Invalid command! please try again .." << std::endl;
	}
}

void protocol::registerUser() {
	std::string name;
	std::cout << "Please enter user name: ";
	std::cin >> name;

	// rsa_keys = encryption
	std::string public_key;
	Request r = Request();
	r.h.code = 1100;
	std::string payload = name + "\0" + public_key;
	r.h.payload_size = payload.length();
	strncpy(r.p.payload, payload.c_str(), payload.length());
}

void protocol::requestClientsList() {
	Request r = Request();
	r.h.code = 1101;
	r.h.payload_size = 0;
}

void protocol::requestPublicKey() {}
void protocol::requestMessages() {}
void protocol::sendMessage() {}
void protocol::requestSymmetricKey() {}
void protocol::sendSymmetricKey() {}
void protocol::exitProgram() {}
