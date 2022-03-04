#include "request.h"

request::request(): r(new Request) {
}

request::request(char* uuid, char version, uint16_t code): request::request(uuid, version, code, "") {
}

request::request(char* uuid, char version, uint16_t code, std::string payload): r(new Request) {
	memcpy(r->h.client_id, uuid, UUID_SIZE);
	r->h.version = version;
	r->h.code = code;
	r->h.payload_size = payload.length();
	r->payload = payload;
}

request::~request() {
	delete r;
}

int request::size() {
	return sizeof(RequestHeader) + r->h.payload_size;
}

std::string request::toString() {
	char* str = new char[size()]{ 0 };

	int offset = 0;
	memcpy(&str[offset], &r->h.client_id, UUID_SIZE);
	offset += UUID_SIZE;
	memcpy(&str[offset], &r->h.version, sizeof(char));
	offset += sizeof(char);
	memcpy(&str[offset], &r->h.code, sizeof(uint16_t));
	offset += sizeof(uint16_t);
	memcpy(&str[offset], &r->h.payload_size, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	if (r->h.payload_size > 0) {
		memcpy(&str[offset], r->payload.c_str(), r->h.payload_size);
		offset += r->h.payload_size;
	}

	memcpy(&str[offset], "\0", 1);

	return std::string(str, size());
}

void request::print() {
	std::cout << "#####################################################################" << std::endl;
	std::cout << "############################### Request #############################" << std::endl;
	std::cout << "client_id:    " << r->h.client_id << std::endl;
	std::cout << "version:      " << (int)r->h.version << std::endl;
	std::cout << "code:         " << r->h.code << std::endl;
	std::cout << "payload_size: " << r->h.payload_size << std::endl;
	std::cout << "payload:      " << r->payload << std::endl << std::endl;
}