#include "message.h"

message::message(std::string target, std::string msgType, std::string msgContent) {
	//r->h.client_id = target;
	r->h.version = profile::getVersion();
	r->h.code = (uint16_t)1103;
	r->payload = target + msgType + std::to_string(msgContent.length()) + msgContent;
	r->h.payload_size = r->payload.length();
}

message::message(std::string target, std::string msgType) {
	//r->h.client_id = target;
	//r->h.version = prof.getVersion();
	r->h.code = 1103;
	r->h.payload_size = 0;
	r->payload = "";
}

