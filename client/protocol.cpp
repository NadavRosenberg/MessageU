#include "protocol.h"
#include "request.h"
#include "message.h"
#include "global.h"

protocol::protocol(connection* c, profile* p): conn(c), prof(p) {
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
	std::getline(std::cin >> std::ws, name);
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

	request* req = new request(prof->getUuid(), prof->getVersion(), 1100, std::string(payload_req, NAME_LENGTH + PUBLIC_KEY_LENGTH));

	// send request & get response
	response* res = sendAndReceive(req);

	// analyze response's payload
	std::string payload_res = res->get_payload();
	char* uuid = new char[UUID_SIZE + 1];
	memcpy(uuid, payload_res.c_str(), UUID_SIZE);
	uuid[UUID_SIZE] = '\0';

	// save user
	prof->setData(name, uuid, keys.private_key);
}

void protocol::requestClientsList() {
    // create request
	request* req = new request(prof->getUuid(), prof->getVersion(), 1101);

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
	std::getline(std::cin >> std::ws, client_id);
	if (client_id.length() != UUID_SIZE) {
		std::cerr << "Error: Invalid client's id!" << std::endl;
		return;
	}
	
	// create request
	request* req = new request(prof->getUuid(), prof->getVersion(), 1102, client_id);

	// send request & get response
	response* res = sendAndReceive(req);

	printf("Requested public key: %s\n", &res->get_payload().c_str()[UUID_SIZE]);
}

void protocol::requestMessages() {
	// create request
	request* req = new request(prof->getUuid(), prof->getVersion(), 1104);

	// send request & get response
	response* res = sendAndReceive(req);
	
	// print messages
	//message* messages = get_messages(req);
	std::string payload_res = res->get_payload();

	char* pchr = new char[res->get_payload_size() + 1]{ 0 };
	memcpy(pchr, payload_res.c_str(), res->get_payload_size());

	int offset = 0;
	while (offset < res->get_payload_size()) {
		printf("From: %.*s\n", UUID_SIZE, &pchr[offset]);
		offset += UUID_SIZE + sizeof(uint32_t);
		printf("Content:\n");

		char msg_type;
		memcpy(&msg_type, &pchr[offset], sizeof(char));
		offset += sizeof(char);

		if (msg_type == '1') {
			offset += sizeof(uint32_t);
			printf("Request for symmetric key\n");
		}
		else if (msg_type == '2') {
			offset += sizeof(uint32_t);
			printf("Symmetric key received\n");
			offset += SYMMETRIC_KEY_LENGTH;
		}
		else if (msg_type == '3') {
			uint32_t content_size;
			memcpy(&content_size, &pchr[offset], sizeof(uint32_t));
			offset += sizeof(uint32_t);
			printf("%.*s\n", content_size, &pchr[offset]);
			offset += content_size;
		}
		else {
			printf("Something went wrong!\n");
			return;
		}
		printf("-----<EOM>-----\n");
	}
}

void protocol::sendMessage() {
	std::string target;
	std::cout << "Please enter user's id to contact: ";
	std::getline(std::cin >> std::ws, target);

	std::string msgContent;
	std::cout << "Please enter message content: ";
	std::getline(std::cin >> std::ws, msgContent);

	// create request
	int payload_size = UUID_SIZE + sizeof(char) + sizeof(uint32_t) + msgContent.length();
	char* payload_req = new char[payload_size]{ 0 };

	int offset = 0;
	memcpy(payload_req, target.c_str(), UUID_SIZE);
	offset += UUID_SIZE;
	payload_req[offset] = '\3';
	offset += sizeof(char);
	uint32_t content_size = msgContent.length();
	memcpy(&payload_req[offset], &content_size, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&payload_req[offset], msgContent.c_str(), msgContent.length());

	request* req = new request(prof->getUuid(), prof->getVersion(), 1103, std::string(payload_req, payload_size));

	// create request
	//message* msg = new message(prof.getUuid(), prof.getVersion(), target, '\3', msgContent);

	// send request & get response
	response* res = sendAndReceive(req);
}

void protocol::requestSymmetricKey() {
	std::string target;
	std::cout << "Please enter user's id to contact: ";
	std::getline(std::cin >> std::ws, target);

	// create request
	int payload_size = UUID_SIZE + sizeof(char) + sizeof(uint32_t);
	char* payload_req = new char[payload_size] { 0 };

	int offset = 0;
	memcpy(payload_req, target.c_str(), UUID_SIZE);
	offset += UUID_SIZE;
	payload_req[offset] = '\1';
	offset += sizeof(char);
	uint32_t content_size = 0;
	memcpy(&payload_req[offset], &content_size, sizeof(uint32_t));

	request* req = new request(prof->getUuid(), prof->getVersion(), 1103, std::string(payload_req, payload_size));

	// create request
	//message* msg = new message(prof.getUuid(), prof.getVersion(), target, '\1');

	// send request & get response
	response* res = sendAndReceive(req);
}

void protocol::sendSymmetricKey() {
	std::string target;
	std::cout << "Please enter user's id to contact: ";
	std::getline(std::cin >> std::ws, target);

	// create request
	int payload_size = UUID_SIZE + sizeof(char) + sizeof(uint32_t) + SYMMETRIC_KEY_LENGTH;
	char* payload_req = new char[payload_size] { 0 };

	int offset = 0;
	memcpy(payload_req, target.c_str(), UUID_SIZE);
	offset += UUID_SIZE;
	payload_req[offset] = '\2';
	offset += sizeof(char);
	uint32_t content_size = SYMMETRIC_KEY_LENGTH;
	memcpy(&payload_req[offset], &content_size, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	rsa_keys keys = encryption::getRSAKeys();
	memcpy(&payload_req[offset], keys.public_key.c_str(), SYMMETRIC_KEY_LENGTH);


	request* req = new request(prof->getUuid(), prof->getVersion(), 1103, std::string(payload_req, payload_size));

	// create request
	//message* msg = new message(prof.getUuid(), prof.getVersion(), target, '\2', "public_key_of_target");

	// send request & get response
	response* res = sendAndReceive(req);
}

void protocol::exitProgram() {
}

response* protocol::sendAndReceive(request* req) {
	conn->sendRequest(req);
	response* res = conn->getResponse();

	if (res->get_code() == 9000) {
		throw std::runtime_error(res->get_payload());
	}

	return res;
}