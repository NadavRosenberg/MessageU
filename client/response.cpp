#include "response.h"

response::response(std::string header_bytes): r(new Response) {
	int offset = 0;
	const char* dptr = header_bytes.c_str();
	memcpy(&r->h.version, dptr, sizeof(char));
	offset += sizeof(char);
	memcpy(&r->h.code, &dptr[offset], sizeof(uint16_t));
	offset += sizeof(uint16_t);
	memcpy(&r->h.payload_size, &dptr[offset], sizeof(uint32_t));
	offset += sizeof(uint32_t);
}

response::~response() {
	delete r;
}

uint16_t response::get_code() {
	return r->h.code;
}

int response::get_payload_size() {
	return r->h.payload_size;
}

void response::set_payload(std::string payload_bytes) {
	r->payload = payload_bytes;
}

std::string response::get_payload() {
	return r->payload;
}

void response::print() {
	std::cout << "#####################################################################" << std::endl;
	std::cout << "############################### Response ############################" << std::endl;
	std::cout << "version:      " << r->h.version << std::endl;
	std::cout << "code:         " << r->h.code << std::endl;
	std::cout << "payload_size: " << r->h.payload_size << std::endl;
	std::cout << "payload:      " << r->payload << std::endl << std::endl;
}