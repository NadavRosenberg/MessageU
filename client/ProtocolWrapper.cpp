#include "ProtocolWrapper.h"
#include "request.h"
#include "message.h"
#include "global.h"
#include "AESWrapper.h"

ProtocolWrapper::ProtocolWrapper(protocol* p, users* u) : ptcl(p), _users(u)
{
}

void ProtocolWrapper::handle(int command_code) {
	try {
		switch (command_code) 
		{
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
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void ProtocolWrapper::registerUser() 
{
	std::string name;
	std::cout << "Please enter user name: ";
	std::getline(std::cin >> std::ws, name);
	if (name.length() > NAME_LENGTH - 1) {
		std::cerr << "Invalid name!" << std::endl;
		return;
	}

	// send request & handle response
	ptcl->send1100(name);
	ptcl->handle2100(name);
}

void ProtocolWrapper::requestClientsList() 
{
	// send request & handle response
	ptcl->send1101();
	ptcl->handle2101();
}

void ProtocolWrapper::requestPublicKey() 
{
	// get user's uuid from name
	std::string user_id = getIdFromName();

	// send request & handle response
	ptcl->send1102(user_id);
	ptcl->handle2102();
}

void ProtocolWrapper::requestMessages() 
{
	ptcl->send1104();
	ptcl->handle2104();
}

void ProtocolWrapper::sendMessage() 
{
	// get user's uuid from name
	std::string user_id = getIdFromName();

	std::string msgContent;
	std::cout << "Please enter message content: ";
	std::getline(std::cin >> std::ws, msgContent);

	// create the message
	message msg(user_id, '\3', msgContent.length(), msgContent);

	// send request & handle response
	ptcl->send1103(msg);
	ptcl->handle2103();
}

void ProtocolWrapper::requestSymmetricKey() 
{
	// get user's uuid from name
	std::string user_id = getIdFromName();

	// create the message
	message msg(user_id, '\1');

	// send request & handle response
	ptcl->send1103(msg);
	ptcl->handle2103();
}

void ProtocolWrapper::sendSymmetricKey() 
{
	// get user's uuid from name
	std::string user_id = getIdFromName();

	// create symmetric key
	unsigned char* buffer = new unsigned char[SYMMETRIC_KEY_LENGTH]{ 0 };
	AESWrapper::GenerateKey(buffer, SYMMETRIC_KEY_LENGTH);
	std::string symm_key(reinterpret_cast<char*>(buffer));

	// save symmetric key
	_users->set_user_symm_key(user_id, symm_key);

	// create the message
	message msg(user_id, '\2', SYMMETRIC_KEY_LENGTH, symm_key);

	// send request & handle response
	ptcl->send1103(msg);
	ptcl->handle2103();
}

void ProtocolWrapper::exitProgram() {
}

std::string ProtocolWrapper::getIdFromName()
{
	std::string username;
	std::cout << "Please enter the user's name: ";
	std::getline(std::cin >> std::ws, username);
	if (username.length() > NAME_LENGTH - 1)
	{
		throw std::runtime_error("Error: Invalid user's name!");
	}

	std::string user_id = _users->get_user_id(username);
	if (user_id.length() == 0)
	{
		throw std::runtime_error("Error: Cannot find the client! try to request clients list ..");
	}

	return user_id;
}