#include "protocol.h"
#include "RSAWrapper.h"
#include "AESWrapper.h"

#define PUBLIC_KEY_LENGTH 160

protocol::protocol(connection* c, profile* p, users* u) : conn(c), prof(p), _users(u)
{
}

void protocol::send1100(std::string name)
{
	// create asymmetric keys
	RSAPrivateWrapper pw;
	prof->setKeys(pw.getPublicKey(), pw.getPrivateKey());

	// create request's payload
	char payload_req[NAME_LENGTH + PUBLIC_KEY_LENGTH]{ 0 };
	memcpy(payload_req, name.c_str(), name.length());
	payload_req[name.length()] = '\0';
	prof->getPublicKey().copy(&payload_req[NAME_LENGTH], PUBLIC_KEY_LENGTH);

	// create & send request
	request req(prof->getUuid(), prof->getVersion(), 1100, std::string(payload_req, NAME_LENGTH + PUBLIC_KEY_LENGTH));
	conn->sendRequest(&req);
}

void protocol::send1101()
{
	request req(prof->getUuid(), prof->getVersion(), 1101);
	conn->sendRequest(&req);
}

void protocol::send1102(std::string client_id)
{
	request req(prof->getUuid(), prof->getVersion(), 1102, client_id);
	conn->sendRequest(&req);
}

void protocol::send1103(message msg)
{
	// get message type
	char msg_type = msg.getType();

	try
	{
		if (msg_type == '\2')
		{
			// get user's public key
			std::string pub_key = _users->get_user_public_key(msg.getClientId());
			if (pub_key.length() != PUBLIC_KEY_LENGTH)
				throw std::runtime_error("Error: Failed fetching public key! try to fetch the key ..");

			// encrypt symmetric key using target's public key
			RSAPublicWrapper pw(pub_key);
			std::string plain = msg.getContent();
			std::string ciper = pw.encrypt(plain);

			// save encrypted content to msg
			msg.setContentSize(ciper.length());
			msg.setContent(ciper);
		}
		else if (msg_type == '\3')
		{
			// get user's symmetric key
			std::string symm_key = _users->get_user_symm_key(msg.getClientId());
			if (symm_key.length() != SYMMETRIC_KEY_LENGTH)
				throw std::runtime_error("Error: Failed fetching symmetric key! try to fetch the key ..");

			// encrypt the message using symmetric key
			AESWrapper aes(reinterpret_cast<const unsigned char*>(symm_key.c_str()), symm_key.length());
			std::string ciper = aes.encrypt(msg.getContent().c_str(), msg.getContentSize());

			// save encrypted content to msg
			msg.setContentSize(ciper.length());
			msg.setContent(ciper);
		}
	}
	catch (int num)
	{
		throw std::runtime_error("Error: Something went wrong! try to fetch the key ..");
	}

	// create request's payload
	std::string payload = msg.toString();

	// create & send request
	request req(prof->getUuid(), prof->getVersion(), 1103, payload);
	conn->sendRequest(&req);
}

void protocol::send1104()
{
	request req(prof->getUuid(), prof->getVersion(), 1104);
	conn->sendRequest(&req);
}

void protocol::handle2100(std::string name)
{
	response* res = getResponse();

	// analyze response's payload
	std::string payload_res = res->get_payload();
	char uuid[UUID_SIZE + 1];
	memcpy(uuid, payload_res.c_str(), UUID_SIZE);
	uuid[UUID_SIZE] = '\0';

	// save user
	prof->setData(name, uuid, prof->getPrivateKey());
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

	//delete[] pchr;

	// print users
	_users->print();
}

void protocol::handle2102()
{
	response* res = getResponse();

	std::string uuid(res->get_payload().c_str(), UUID_SIZE);
	std::string pub_key(&res->get_payload().c_str()[UUID_SIZE], PUBLIC_KEY_LENGTH);

	try 
	{
		_users->set_user_public_key(uuid, pub_key);
		printf("Request for public key ended successfully!\n");
	}
	catch (int num)
	{
		printf("Failed to fetch public key!\n");
	}
}

void protocol::handle2103()
{
	response* res = getResponse();
}

void protocol::handle2104()
{
	response* res = getResponse();
	std::string payload = res->get_payload();
	char* pchr = &payload[0];

	int offset = 0;
	while (offset < res->get_payload_size()) {
		// parse message
		message msg(&pchr[offset]);
		offset += msg.size();

		// get user's name
		std::string username = _users->get_username(msg.getClientId());

		// get message type
		char msg_type = msg.getType();

		if (username.length() == 0)
			printf("From: %.*s   <request for clients list to reveal>\n", UUID_SIZE, msg.getClientId().c_str());
		else
			printf("From: %s\n", username.c_str());
		printf("Content:\n");

		if (msg_type == '1') {
			handleMsgType1(msg);
		}
		else if (msg_type == '2') {
			handleMsgType2(msg);
		}
		else if (msg_type == '3') {
			handleMsgType3(msg);
		}
		else {
			printf("Something went wrong!\n");
			return;
		}
		printf("-----<EOM>-----\n");
	}
}

void protocol::handleMsgType1(message msg)
{
	printf("Request for symmetric key\n");
}

void protocol::handleMsgType2(message msg)
{
	try
	{
		// get symmetric key
		std::string ciper = msg.getContent();

		// decrypt the message using private key
		RSAPrivateWrapper pw(prof->getPrivateKey());
		std::string plain_recovered = pw.decrypt(ciper);

		// save symmetric key
		_users->set_user_symm_key(msg.getClientId(), plain_recovered);

		printf("Symmetric key received\n");
	}
	catch (int num)
	{
		printf("Error: Failed fetching symmetric key! try to fetch the key ..\n");
	}
}

void protocol::handleMsgType3(message msg)
{
	// get user's symmetric key
	std::string symm_key = _users->get_user_symm_key(msg.getClientId());
	if (symm_key.length() != SYMMETRIC_KEY_LENGTH)
		printf("Error: Failed fetching symmetric key! try to fetch the key ..\n");

	// decrypt the message using symmetric key
	AESWrapper aes(reinterpret_cast<const unsigned char*>(symm_key.c_str()), symm_key.length());
	std::string ciper = msg.getContent();
	std::string plain = aes.decrypt(ciper.c_str(), ciper.length());

	// print the message
	printf("%s\n", plain.c_str());
}

response* protocol::getResponse()
{
	response* res = conn->getResponse();

	if (res->get_code() == 9000) {
		throw std::runtime_error("Server responed with an error: " + res->get_payload());
	}

	return res;
}