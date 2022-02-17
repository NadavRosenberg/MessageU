#pragma once
#include <boost/asio.hpp>
#include <iostream>

class connection {
private:
	std::string host;
	std::string port;
public:
	connection(std::string, std::string);
	void connect();
	std::string getData(boost::asio::ip::tcp::socket& socket);
	void sendData(boost::asio::ip::tcp::socket& socket, const std::string& message);
};

