#include "protocol.h"
#include "RSAWrapper.h"

protocol::protocol(connection* c, profile* p, users* u) : conn(c), prof(p), _users(u)
{
	RSAPrivateWrapper pw;
	public_key = pw.getPublicKey();
	private_key = pw.getPrivateKey();
}

void protocol::send1100(std::string name)
{
	// create request's payload
	char* payload_req = new char[NAME_LENGTH + PUBLIC_KEY_LENGTH]{ 0 };
	memcpy(payload_req, name.c_str(), name.length());
	payload_req[name.length()] = '\0';
	memcpy(&payload_req[NAME_LENGTH], public_key.c_str(), PUBLIC_KEY_LENGTH);

	// create & send request
	request* req = new request(prof->getUuid(), prof->getVersion(), 1100, std::string(payload_req, NAME_LENGTH + PUBLIC_KEY_LENGTH));
	conn->sendRequest(req);
}

void protocol::send1101()
{
	request* req = new request(prof->getUuid(), prof->getVersion(), 1101);
	conn->sendRequest(req);
}

void protocol::send1102(std::string client_id)
{
	request* req = new request(prof->getUuid(), prof->getVersion(), 1102, client_id);
	conn->sendRequest(req);
}

void protocol::send1103(message msg)
{
	// add symmetric key (if needed)
	if (msg.getType() == '\2')
	{
		msg.setContentSize(SYMMETRIC_KEY_LENGTH);
		msg.setContent(public_key);
	}

	// ecrypt the message
	RSAPublicWrapper pw(public_key);
	std::string plain = msg.getContent();
	std::string ciper = pw.encrypt(plain);
	msg.setContent(ciper);

	// create request's payload
	int payload_size = UUID_SIZE + sizeof(char) + sizeof(uint32_t) + ciper.length();
	char* payload_req = new char[payload_size] { 0 };

	int offset = 0;
	memcpy(payload_req, msg.getClientId().c_str(), UUID_SIZE);
	offset += UUID_SIZE;
	payload_req[offset] = msg.getType();
	offset += sizeof(char);
	uint32_t content_size = ciper.length();
	memcpy(&payload_req[offset], &content_size, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&payload_req[offset], ciper.c_str(), ciper.length());

	// create & send request
	request* req = new request(prof->getUuid(), prof->getVersion(), 1103, std::string(payload_req, payload_size));
	conn->sendRequest(req);
}

void protocol::send1104()
{
	request* req = new request(prof->getUuid(), prof->getVersion(), 1104);
	conn->sendRequest(req);
}

void protocol::handle2100(std::string name)
{
	response* res = getResponse();

	// analyze response's payload
	std::string payload_res = res->get_payload();
	char* uuid = new char[UUID_SIZE + 1];
	memcpy(uuid, payload_res.c_str(), UUID_SIZE);
	uuid[UUID_SIZE] = '\0';

	// save user
	prof->setData(name, uuid, private_key);
}

void protocol::handle2101()
{
	response* res = getResponse();

	// analyze response's payload
	std::string payload_res = res->get_payload();
	int amount = res->get_payload_size() / (UUID_SIZE + NAME_LENGTH);
	char* pchr = new char[res->get_payload_size() + 1]{ 0 };
	memcpy(pchr, payload_res.c_str(), res->get_payload_size());

	// save users
	int offset = 0;
	for (int i = 0; i < amount; i++) {
		std::string uuid = std::string(&pchr[offset], UUID_SIZE);
		std::string name = std::string(&pchr[offset + UUID_SIZE], NAME_LENGTH);
		_users->add_user(uuid, name);
		offset += UUID_SIZE + NAME_LENGTH;
	}

	// print users
	_users->print();
}

void protocol::handle2102()
{
	response* res = getResponse();
	printf("Requested public key: %s\n", &res->get_payload().c_str()[UUID_SIZE]);
}

void protocol::handle2103()
{
	response* res = getResponse();
}

void protocol::handle2104()
{
	response* res = getResponse();

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
			offset += sizeof(uint32_t) + sizeof(char) + sizeof(char);
			printf("Symmetric key received\n");
			offset += SYMMETRIC_KEY_LENGTH;
		}
		else if (msg_type == '3') {
			// get content size
			uint32_t content_size;
			memcpy(&content_size, &pchr[offset], sizeof(uint32_t));
			offset += sizeof(uint32_t);

			// decrypt the message
			RSAPrivateWrapper pw(private_key);
			std::string ciper(pchr, content_size);
			std::string plain = pw.decrypt(ciper);

			printf("%s\n", plain.c_str());
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

response* protocol::getResponse()
{
	response* res = conn->getResponse();

	if (res->get_code() == 9000) {
		throw std::runtime_error("Server responed with an error: " + res->get_payload());
	}

	return res;
}