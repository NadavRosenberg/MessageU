#pragma once
#include <iostream>
#include <string>

#pragma pack(push, 1)
struct RequestHeader
{
	char client_id[16];
	char version;
	uint16_t code;
	uint32_t payload_size;
};

struct RequestPayload
{
	char* payload;
};

struct Request
{
	RequestHeader h;
	RequestPayload p;
};
#pragma pack(pop)
