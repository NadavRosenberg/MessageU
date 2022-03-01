#pragma once
#include <iostream>

class message
{
	std::string client_id;
	char msg_type;
	uint32_t content_size;
	std::string content;
public:
	//message(char* uuid, char version, std::string target, char msgType, std::string msgContent);
	//message(char* uuid, char version, std::string target, char msgType);
	message(std::string client_id, char msg_type, uint32_t content_size, std::string content);
	message(std::string client_id, char msg_type);
	std::string getClientId();
	char getType();
	std::string getContent();
	void setContent(std::string);
	void setContentSize(uint32_t);
};

