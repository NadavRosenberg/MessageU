#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include "utils.h"
#include "request.h"
#include "response.h"

#define HEADER_SIZE 7

class connection {
private:
	std::string host;
	std::string port;
	boost::asio::io_context io_context;
	boost::asio::ip::tcp::socket s;
	boost::asio::ip::tcp::socket& sock;
public:
	connection(std::string, std::string);
	boost::asio::ip::tcp::socket& connect();
	response* getResponse();
	void sendRequest(request*);
};