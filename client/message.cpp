#include "message.h"
#include "global.h"

//message::message(char* uuid, char version, std::string target, char msgType, std::string msgContent) {
//	int payload_size = UUID_SIZE + sizeof(char) + sizeof(uint32_t) + msgContent.length();
//	char* payload = new char[payload_size]{ 0 };
//
//	int offset = 0;
//	memcpy(payload, target.c_str(), UUID_SIZE);
//	offset += UUID_SIZE;
//	payload[offset] = msgType;
//	offset += sizeof(char);
//	int content_size = msgContent.length();
//	memcpy(&payload[offset], &content_size, sizeof(uint32_t));
//	offset += sizeof(uint32_t);
//	memcpy(&payload[offset], msgContent.c_str(), content_size);
//
//	memcpy(r->h.client_id, uuid, UUID_SIZE);
//	r->h.version = version;
//	r->h.code = 1103;
//	r->h.payload_size = payload_size;
//	r->payload = payload;
//}
//
//message::message(char* uuid, char version, std::string target, char msgType): message::message(uuid, version, target, msgType, "") {
//}
//
//message::message(std::string target, std::string msgType) {
//
//	//r->h.client_id = target;
//	//r->h.version = prof.getVersion();
//	r->h.code = 1103;
//	r->h.payload_size = 0;
//	r->payload = "";
//}

message::message(std::string ci, char mt, uint32_t cs, std::string c) :
	client_id(ci), msg_type(mt), content_size(cs), content(c)
{
}

message::message(std::string ci, char mt) :
	client_id(ci), msg_type(mt), content_size(0), content("")
{
}

std::string message::getClientId()
{
	return client_id;
}

char message::getType()
{
	return msg_type;
}

std::string message::getContent()
{
	return content;
}

void message::setContent(std::string c)
{
	content = c;
}

void message::setContentSize(uint32_t _content_size)
{
	content_size = _content_size;
}


