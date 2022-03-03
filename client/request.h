#pragma once
#include <iostream>
#include <string>
#include "global.h"
#include "profile.h"

#pragma pack(push, 1)
struct RequestHeader {
	char client_id[UUID_SIZE + 1];
	char version = '\0';
	uint16_t code;
	uint32_t payload_size;
};

struct Request {
	RequestHeader h;
	std::string payload;
};
#pragma pack(pop)

class request {
protected:
	Request* r;
	request();
public:
	request(char* uuid, char version, uint16_t code);
	request(char* uuid, char version, uint16_t code, std::string payload);
	~request();
	int size();
	std::string toString();
	void print();
};
