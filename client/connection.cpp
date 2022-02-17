#include "connection.h"

connection::connection(std::string h, std::string p): host(h), port(p) {
}

std::string connection::getData(boost::asio::ip::tcp::socket& socket)
{
	boost::asio::streambuf buf;
	boost::asio::read_until(socket, buf, "\n");
	std::string data = boost::asio::buffer_cast<const char*>(buf.data());
	return data;
}

void connection::sendData(boost::asio::ip::tcp::socket& socket, const std::string& message)
{
	boost::asio::write(socket, boost::asio::buffer(message + "\n"));
}

void connection::connect() {
	boost::asio::io_context io_context;

	// creating socket object for client
	boost::asio::ip::tcp::socket s(io_context);

	boost::asio::ip::tcp::resolver resolver(io_context);

	// invoking connect method of socket object to initiate connection with server
	boost::asio::connect(s, resolver.resolve(host, port));
}