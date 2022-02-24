#pragma once
#include <iostream>

#pragma pack(push, 1)
struct ResponseHeader {
	char version;
	uint16_t code;
	uint32_t payload_size;
};

struct Response {
	ResponseHeader h;
	std::string payload;
};
#pragma pack(pop)

class response {
protected:
	Response* r;
public:
	response(std::string&);
	~response();
	int get_payload_size();
	void set_payload(std::string);
	std::string get_payload();
	void print();
};