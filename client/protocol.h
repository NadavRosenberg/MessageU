#pragma once
#include <iostream>
#include "connection.h"
#include "encryption.h"
#include "profile.h"

class protocol
{
private:
	profile prof;
	connection* conn;
	void registerUser();
	void requestClientsList();
	void requestPublicKey();
	void requestMessages();
	void sendMessage();
	void requestSymmetricKey();
	void sendSymmetricKey();
	void exitProgram();
public:
	protocol(connection*);
	void handle(int);
};

