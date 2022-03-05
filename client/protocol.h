#pragma once
#include <iostream>
#include "message.h"
#include "connection.h"
#include "users.h"

class protocol
{
private:
	connection* conn;
	profile* prof;
	users* _users;
	response* getResponse();
	void handleMsgType1(message);
	void handleMsgType2(message);
	void handleMsgType3(message);
public:
	protocol(connection*, profile*, users*);
	void send1100(std::string);
	void send1101();
	void send1102(std::string);
	void send1103(message);
	void send1104();
	void handle2100(std::string);
	void handle2101();
	void handle2102();
	void handle2103();
	void handle2104();
};
