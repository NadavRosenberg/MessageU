#pragma once
#include "request.h"

//enum MSG_TYPE { REQUEST_SYMMETRIC_KEY = 1, SEND_SYMMETRIC_KEY, TEXT_MESSAGE };

class message : public request
{
public:
	message(std::string target, std::string msgType, std::string msgContent);
	message(std::string target, std::string msgType);
};

