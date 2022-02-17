#pragma once
#include <iostream>

class Commands
{
public:
	void registerUser();
	void requestClientsList();
	void requestPublicKey();
	void requestMessages();
	void sendMessage();
	void requestSymmetricKey();
	void sendSymmetricKey();
	void exitProgram();
};

