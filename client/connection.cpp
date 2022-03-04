#include "connection.h"
#include "global.h"

connection::connection(std::string h, std::string p): host(h), port(p), s(io_context), sock(connect()) {
}

response* connection::getResponse()
{
	boost::asio::streambuf read_buffer;
	auto bytes_transferred = boost::asio::read(sock, read_buffer, boost::asio::transfer_exactly(HEADER_SIZE));
	std::string header_bytes = std::string(boost::asio::buffer_cast<const char*>(read_buffer.data()), HEADER_SIZE);
	read_buffer.consume(bytes_transferred);
	response* res = new response(header_bytes);

	bytes_transferred = boost::asio::read(sock, read_buffer, boost::asio::transfer_exactly(res->get_payload_size()));
	std::string payload_bytes = std::string(boost::asio::buffer_cast<const char*>(read_buffer.data()), res->get_payload_size());
	read_buffer.consume(bytes_transferred);
	res->set_payload(payload_bytes);

	if (DEBUG_MODE)
		res->print();

	return res;
}

void connection::sendRequest(request* req) {
	if (DEBUG_MODE)
		req->print();

	const std::string& message = req->toString();
	boost::asio::write(sock, boost::asio::buffer(message, req->size()));
}

boost::asio::ip::tcp::socket& connection::connect() {
	try {
		boost::asio::ip::tcp::resolver resolver(io_context);

		// invoking connect method of socket object to initiate connection with server
		boost::asio::connect(s, resolver.resolve(host, port));

		return s;
	}
	catch (...) {
		std::cerr << "Failed to connect to server! Please try again later .." << std::endl;
		exit_program();
	}
}