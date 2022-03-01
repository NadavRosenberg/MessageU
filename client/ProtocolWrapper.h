#pragma once
#include <iostream>
#include "connection.h"
#include "profile.h"
#include "protocol.h"

class ProtocolWrapper
{
private:
	protocol* ptcl;
	profile* prof;
	connection* conn;
	users* _users;
	void registerUser();
	void requestClientsList();
	void requestPublicKey();
	void requestMessages();
	void sendMessage();
	void requestSymmetricKey();
	void sendSymmetricKey();
	void exitProgram();
	std::string getIdFromName();
public:
	ProtocolWrapper(protocol*, users*);
	void handle(int);
};