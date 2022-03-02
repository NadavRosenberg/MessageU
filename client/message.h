#pragma once
#include <iostream>

class message
{
	std::string client_id;
	uint32_t msg_id;
	char msg_type;
	uint32_t content_size;
	std::string content;
public:
	message(char*);
	message(std::string client_id, char msg_type, uint32_t content_size, std::string content);
	message(std::string client_id, char msg_type);
	std::string getClientId();
	char getType();
	uint32_t getContentSize();
	std::string getContent();
	void setContent(std::string);
	void setContentSize(uint32_t);
	int size();
};

