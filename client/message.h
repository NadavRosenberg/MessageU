#pragma once
#include "request.h"

//enum MSG_TYPE { REQUEST_SYMMETRIC_KEY = 1, SEND_SYMMETRIC_KEY, TEXT_MESSAGE };

class message : public request
{
public:
	message(char* uuid, char version, std::string target, char msgType, std::string msgContent);
	message(char* uuid, char version, std::string target, char msgType);
	message(std::string target, std::string msgType);
};

