#include "protocol.h"
#include "request.h"
#include "message.h"
#include "global.h"

protocol::protocol(connection* c): conn(c), prof(profile()) {
}

void protocol::handle(int command_code) {
	try {
		switch (command_code) {
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
				std::cerr << "Invalid command! please try again .." << std::endl;
		}
	}
	catch (const std::exception& e)	{
		std::cout << "Server responed with an error: " << e.what() << std::endl;
	}
}

void protocol::registerUser() {
	std::string name;
	std::cout << "Please enter user name: ";
	std::cin >> name;
	if (name.length() > NAME_LENGTH - 1) {
		std::cerr << "Invalid name!" << std::endl;
		return;
	}

	rsa_keys keys = encryption::getRSAKeys();

	// create request
	char* payload_req = new char[NAME_LENGTH + PUBLIC_KEY_LENGTH]{ 0 };
	memcpy(payload_req, name.c_str(), name.length());
	payload_req[NAME_LENGTH] = '\0';
	memcpy(&payload_req[NAME_LENGTH], keys.public_key.c_str(), PUBLIC_KEY_LENGTH);

	request* req = new request(prof.getUuid(), prof.getVersion(), 1100, std::string(payload_req, NAME_LENGTH + PUBLIC_KEY_LENGTH));

	// send request & get response
	response* res = sendAndReceive(req);

	// analyze response's payload
	std::string payload_res = res->get_payload();
	char* uuid = new char[16];
	memcpy(uuid, payload_res.c_str(), 16);

	// save user
	prof.setData(name, uuid, keys.private_key);
}

void protocol::requestClientsList() {
    // create request
	request* req = new request(prof.getUuid(), prof.getVersion(), 1101);

	// send request & get response
	response* res = sendAndReceive(req);

	// analyze response's payload
	std::string payload_res = res->get_payload();
	int amount = res->get_payload_size() / (UUID_SIZE + NAME_LENGTH);
	char* pchr = new char[res->get_payload_size() + 1]{ 0 };
	memcpy(pchr, payload_res.c_str(), res->get_payload_size());

	// print users
	std::cout << "Users:" << std::endl;
	for (int i = 0; i < amount; i++) {
		printf("# %.*s ", NAME_LENGTH, pchr + UUID_SIZE);
		printf("(%.*s)\n", UUID_SIZE, pchr);
		pchr += UUID_SIZE + NAME_LENGTH;
	}
}

void protocol::requestPublicKey() {
	std::string client_id;
	std::cout << "Please enter the client's id: ";
	std::cin >> client_id;
	if (client_id.length() != UUID_SIZE) {
		std::cerr << "Error: Invalid client's id!" << std::endl;
		return;
	}
	
	// create request
	request* req = new request(prof.getUuid(), prof.getVersion(), 1102, client_id);

	// send request & get response
	response* res = sendAndReceive(req);

	printf("Requested public key: %s\n", &res->get_payload().c_str()[UUID_SIZE]);
}

void protocol::requestMessages() {
	request r(1102);
	//conn->sendRequest(r);
	//response res = conn->getResponse();
}

void protocol::sendMessage() {
	std::string target;
	std::cout << "Please enter user to contact: ";
	std::cin >> target;

	std::string msgContent;
	std::cout << "Please enter message content: ";
	std::cin >> msgContent;
		
	message m(target, "3", msgContent);
	//conn->sendRequest(m);
	//response res = conn->getResponse();
}

void protocol::requestSymmetricKey() {
	std::string target;
	std::cout << "Please enter user to contact: ";
	std::cin >> target;

	message m(target, "1");
	//conn->sendRequest(m);
	//response res = conn->getResponse();
}

void protocol::sendSymmetricKey() {
	std::string target;
	std::cout << "Please enter user to contact: ";
	std::cin >> target;

	std::string key = encryption::getEncryptedSymmetricKey();

	message m(target, "2", key);
	//conn->sendRequest(m);
	//response res = conn->getResponse();
}

void protocol::exitProgram() {
}

response* protocol::sendAndReceive(request* req) {
	conn->sendRequest(*req);
	response* res = conn->getResponse();

	if (res->get_code() == 9000) {
		throw std::runtime_error(res->get_payload());
	}

	return res;
}