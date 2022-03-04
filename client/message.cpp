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

message::message(char* payload)
{
	int offset = 0;

	// get user's uuid
	client_id = std::string(&payload[offset], UUID_SIZE);
	offset += UUID_SIZE;

	// get message id
	memcpy(&msg_id, &payload[offset], sizeof(uint32_t));
	offset += sizeof(uint32_t);

	// get message type
	msg_type = payload[offset];
	offset += sizeof(char);

	// get content size
	memcpy(&content_size, &payload[offset], sizeof(uint32_t));
	offset += sizeof(uint32_t);

	// get content
	if (content_size > 0)
		content = std::string(&payload[offset], content_size);
}

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

uint32_t message::getContentSize()
{
	return content_size;
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

int message::size()
{
	return UUID_SIZE + sizeof(uint32_t) + sizeof(char) + sizeof(uint32_t) + content_size;
}


