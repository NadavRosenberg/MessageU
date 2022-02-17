#pragma once
#include <iostream>
#include "connection.h"
#include "request.h"
#include "encryption.h"

class protocol
{
private:
	connection conn;
	void registerUser();
	void requestClientsList();
	void requestPublicKey();
	void requestMessages();
	void sendMessage();
	void requestSymmetricKey();
	void sendSymmetricKey();
	void exitProgram();
public:
	protocol(connection);
	void handle(int);
};

