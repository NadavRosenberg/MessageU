#pragma once
#include <iostream>
#include <string>

#pragma pack(push, 1)
struct RequestHeader
{
	uint32_t user_id;
	char version;
	char op;
	uint16_t name_len;
	char *filename;
};

struct RequestPayload
{
	uint32_t size;
	char* payload;
};

struct Request
{
	RequestHeader h;
	RequestPayload p;
};
#pragma pack(pop)
