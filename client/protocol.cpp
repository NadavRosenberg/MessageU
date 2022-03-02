#include "protocol.h"
#include "RSAWrapper.h"
#include "AESWrapper.h"
#include "Base64Wrapper.h"

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
	public_key.copy(&payload_req[NAME_LENGTH], PUBLIC_KEY_LENGTH);
	//memcpy(&payload_req[NAME_LENGTH], public_key.c_str(), PUBLIC_KEY_LENGTH);

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
	// get message type
	char msg_type = msg.getType();

	try
	{
		if (msg_type == '\2')
		{
			// get user's public key
			std::string pub_key = _users->get_user_public_key(msg.getClientId());

			if (pub_key.length() != PUBLIC_KEY_LENGTH)
			{
				throw std::runtime_error("Error: Failed fetching public key! try to fetch the key ..");
			}

			// ecrypt the symmetric key using public key
			RSAPublicWrapper pw(pub_key);
			const char* plain = msg.getContent().c_str();
			std::string ciper = pw.encrypt(plain, SYMMETRIC_KEY_LENGTH);

			// save encrypted content to msg
			msg.setContentSize(SYMMETRIC_KEY_LENGTH);
			msg.setContent(ciper);
		}
		else if (msg_type == '\3')
		{
			// get user's symmetric key
			std::string symm_key = _users->get_user_symm_key(msg.getClientId());

			if (symm_key.length() != SYMMETRIC_KEY_LENGTH)
			{
				throw std::runtime_error("Error: Failed fetching symmetric key! try to fetch the key ..");
			}

			// encrypt the message using symmetric key
			AESWrapper aes(reinterpret_cast<const unsigned char*>(symm_key.c_str()), SYMMETRIC_KEY_LENGTH);
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
	int payload_size = UUID_SIZE + sizeof(char) + sizeof(uint32_t) + msg.getContentSize();
	char* payload_req = new char[payload_size] { 0 };

	int offset = 0;
	memcpy(payload_req, msg.getClientId().c_str(), UUID_SIZE);
	offset += UUID_SIZE;
	payload_req[offset] = msg.getType();
	offset += sizeof(char);
	uint32_t content_size = msg.getContentSize();
	memcpy(&payload_req[offset], &content_size, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&payload_req[offset], msg.getContent().c_str(), msg.getContentSize());

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

	// convert private to to base64
	std::string pkey = Base64Wrapper::encode(private_key);

	// save user
	prof->setData(name, uuid, pkey);
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
			offset += sizeof(uint32_t);
			printf("Request for symmetric key\n");
		}
		else if (msg_type == '2') {
			try
			{
				// get symmetric key
				std::string ciper_symm_key = msg.getContent();

				// decrypt the message
				RSAPrivateWrapper pw(private_key);
				std::string plain_symm_key = pw.decrypt(ciper_symm_key);

				// save symmetric key
				_users->set_user_symm_key(msg.getClientId(), plain_symm_key);

				printf("Symmetric key received\n");
			}
			catch (int num)
			{
				printf("Failed to fetch Symmetric key!\n");
			}
		}
		else if (msg_type == '3') {
			// get content size
			uint32_t content_size;
			memcpy(&content_size, &pchr[offset], sizeof(uint32_t));
			offset += sizeof(uint32_t);

			// decrypt the message
			RSAPrivateWrapper pw(private_key);
			std::string ciper(&pchr[offset], content_size);
			offset += content_size;
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