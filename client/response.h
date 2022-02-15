#pragma once
#include <iostream>

#pragma pack(push, 1)
struct ResponseHeader {
	char version;
	uint16_t status;
	uint16_t name_len;
	char *filename;
};

struct ResponsePayload
{
	uint32_t size;
	char* payload;
};

struct Response {
	ResponseHeader h;
	ResponsePayload p;
};
#pragma pack(pop)
